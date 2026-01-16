# PhuongTH33 Music Player - UML Documentation

## Class Diagram

```mermaid
classDiagram
    class Song {
        +int id
        +string title
        +string artist
        +string album
        +size_t duration
        +string filePath
        -static getNextId() int
    }
    
    class MusicLibrary {
        -vector~Song~ songs
        -unordered_map~int, Song*~ songIndexByID
        -map~string, Song*~ songIndexByTitle
        -unordered_map~string, vector~Song*~~ artistIndex
        -map~string, vector~Song*~~ albumIndex
        +addSong(song) bool
        +getAllSongs() vector~Song~
        +getSize() size_t
        +findSongByID(id) Song*
        +findSongByTitle(title) Song*
        +findSongByArtist(artist) vector~Song*~
        +getArtistIndex() unordered_map
        +getAlbumIndex() map
        +getSortedSongs() vector~Song*~
        +clear()
    }
    
    class PlaybackQueue {
        -list~Song*~ queue
        +addSong(song)
        +removeSong(songID)
        +clearQueue()
        +getNextSong() Song*
        +getQueueList() list~Song*~
    }
    
    class PlaybackHistory {
        -stack~Song*~ history
        -vector~Song*~ forward
        +getPreviousSong() Song*
        +getForwardSong() Song*
        +addSongToHistory(song)
        +addSongToForward(song)
        +getHistoryList() vector~Song*~
        +getForwardList() vector~Song*~
    }
    
    class ShuffleManager {
        -vector~Song*~ shuffleQueue
        -unordered_set~Song*~ shuffleHistory
        -size_t index
        +enableShuffle(queue)
        +shuffleAll()
        +getNextSong() Song*
        +getShuffleList() vector~Song*~
        +getCurrentIndex() size_t
    }
    
    class AudioEngine {
        -ma_engine engine
        -ma_sound sound
        +init()
        +playFile(filepath)
        +isPlaying() bool
        +isFinished() bool
        +togglePause()
        +seekTo(seconds)
        +getCurrentTime() float
        +getTotalDuration() float
    }
    
    class MusicPlayer {
        -MusicLibrary library
        -PlaybackQueue queue
        -ShuffleManager shuffleMgr
        -PlaybackHistory stack
        -Song* current
        -bool isShuffleEn
        +AudioEngine engine
        +loadLibrary(path)
        +playSong()
        +playNext()
        +playPrevious()
        +enableShuffle()
        +disableShuffle()
    }

    MusicPlayer *-- MusicLibrary
    MusicPlayer *-- PlaybackQueue
    MusicPlayer *-- ShuffleManager
    MusicPlayer *-- PlaybackHistory
    MusicPlayer *-- AudioEngine
    MusicLibrary o-- Song
```

---

## Sequence Diagrams

### Play Next Song

```mermaid
sequenceDiagram
    participant UI as UIComponents
    participant MP as MusicPlayer
    participant PH as PlaybackHistory
    participant SM as ShuffleManager
    participant PQ as PlaybackQueue
    participant AE as AudioEngine

    UI->>MP: playNext()
    MP->>PH: addSongToHistory(current)
    
    alt Shuffle Enabled
        MP->>SM: getNextSong()
        SM-->>MP: next song*
    else Normal Mode
        MP->>PQ: getNextSong()
        PQ-->>MP: next song*
    end
    
    MP->>MP: current = next
    MP->>AE: playFile(current->filePath)
    AE->>AE: ma_sound_init_from_file_w()
    AE->>AE: ma_sound_start()
```

### Play Previous Song

```mermaid
sequenceDiagram
    participant UI as UIComponents
    participant MP as MusicPlayer
    participant PH as PlaybackHistory
    participant AE as AudioEngine

    UI->>MP: playPrevious()
    MP->>PH: getPreviousSong()
    PH-->>MP: prev song*
    
    alt prev != null
        MP->>PH: addSongToForward(current)
        MP->>MP: current = prev
        MP->>AE: playFile(current->filePath)
    else No history
        MP-->>UI: Do nothing
    end
```

### Enable Shuffle

```mermaid
sequenceDiagram
    participant UI as UIComponents
    participant MP as MusicPlayer
    participant SM as ShuffleManager
    participant PQ as PlaybackQueue

    UI->>MP: enableShuffle()
    MP->>PQ: getQueueList()
    PQ-->>MP: queue list
    MP->>SM: enableShuffle(queue)
    SM->>SM: shuffleQueue.clear()
    SM->>SM: Copy all songs
    SM->>SM: shuffleAll()
    MP->>MP: isShuffleEn = true
```

### Load Library

```mermaid
sequenceDiagram
    participant UI as UIComponents
    participant MP as MusicPlayer
    participant ML as MusicLibrary
    participant FS as filesystem

    UI->>MP: loadLibrary(path)
    MP->>FS: iterate directory
    
    loop For each .mp3/.wav
        MP->>MP: loadMetadata(filepath, song)
        Note right of MP: Uses TagLib + fs::u8path()
        MP->>ML: addSong(song)
        ML->>ML: Update songIndexByID
        ML->>ML: Update songIndexByTitle
        ML->>ML: Update artistIndex
        ML->>ML: Update albumIndex
    end
```

---

## Threading Model

| Component | Thread | Implementation |
|-----------|--------|----------------|
| UI Rendering | Main | GLFW/ImGui loop |
| Audio Playback | Background | miniaudio internal |
| Audio Decoding | Async | `MA_SOUND_FLAG_ASYNC` |
| File Loading | Main | Synchronous |

---

## Data Flow

```
User clicks song → MusicPlayer.chooseAndPlaySong()
                 → PlaybackHistory.addSongToHistory()
                 → AudioEngine.playFile()
                 → miniaudio (background thread)
                 → Audio output
```
