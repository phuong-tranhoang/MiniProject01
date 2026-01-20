// MusicPlayer.cpp

#include "../include/MusicPlayer.h"
#include <filesystem>
#include <iostream>
#include <queue>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace fs = std::filesystem;

MusicPlayer::MusicPlayer() { engine.init(); }

MusicPlayer::~MusicPlayer() {}

void MusicPlayer::loadLibrary(const std::string &path) {
  if (!fs::exists(path)) {
    std::cerr << "Error: Path does not exist -> " << path << std::endl;
    return;
  }

  // Check if folder already loaded
  std::string normalizedPath = fs::canonical(path).string();
  if (loadedFolders.count(normalizedPath) > 0) {
    std::cout << "Folder already loaded: " << path << std::endl;
    return;
  }
  loadedFolders.insert(normalizedPath);

  int count = 0;

  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.path().extension() == ".mp3" ||
        entry.path().extension() == ".wav") {
      Song newSong;
      newSong.filePath = entry.path().string();

      // Default metadata
      newSong.title = entry.path().stem().string();
      newSong.artist = "Unknown Artist";
      newSong.album = "Unknown Album";

      loadMetadata(newSong.filePath, newSong);

      library.addSong(newSong);
      count++;
    }
  }
  std::cout << "Loaded: " << count << " songs from " << path << std::endl;
}

const MusicLibrary &MusicPlayer::getLibrary() const { return library; }

void MusicPlayer::loadMetadata(const std::string &filePath, Song &newSong) {
  // Use fs::u8path for proper UTF-8 to wchar_t conversion on Windows
  fs::path fsPath = fs::u8path(filePath);
  TagLib::FileRef f(fsPath.wstring().c_str());

  if (!f.isNull() && f.tag()) {
    TagLib::Tag *tag = f.tag();
    newSong.title = tag->title().to8Bit(true); // Convert String to std::string
    newSong.artist = tag->artist().to8Bit(true);
    newSong.album = tag->album().to8Bit(true);
    newSong.duration = f.audioProperties()->lengthInSeconds();
  }
}

size_t MusicPlayer::getLibrarySize() { return library.getSize(); }

const Song *MusicPlayer::getCurrentSong() { return current; }

const PlaybackQueue &MusicPlayer::getQueueManager() const { return queue; }

const PlaybackHistory &MusicPlayer::getHistoryManager() const { return stack; }

void MusicPlayer::addSongToQueue(const Song *song) {
  queue.addSong(song);
  if (isShuffleEn == true) {
    shuffleMgr.addSong(song);
  }
}

void MusicPlayer::removeSongFromQueue(const Song *song) {
  queue.removeSong(song);
}

size_t MusicPlayer::getQueueSize() { return queue.getQueueSize(); }

void MusicPlayer::playSong() {
  if (current == nullptr) {
    std::cout << "No song to play" << std::endl;
    return;
  }
  engine.playFile(current->filePath); // play the song
}

void MusicPlayer::chooseAndPlaySong(const Song *song) {
  if (current != nullptr) {
    stack.addSongToHistory(current); // push current song into history
  }
  current = song; // play the newly chosen song
  playSong();
}

void MusicPlayer::playNext() {
  if (current != nullptr) {
    stack.addSongToHistory(current); // add current song to history
  }

  const Song *next = nullptr;

  if (stack.getForwardSize() != 0) {
    next = stack.getForwardSong();
  } else if (isShuffleEn) {
    next = shuffleMgr.getNextSong();
  } else {
    next = queue.getNextSong();
  }
  if (next != nullptr) {
    current = next;
    playSong();
  } else {
    std::cout << "No Songs in Queue" << std::endl;
    if (current != nullptr) {
      ma_sound_stop(&engine.sound);
      ma_sound_uninit(&engine.sound);
      engine.isSoundLoaded = false;
      engine.songFinished = true;
    }
    current = nullptr;
  }
}

void MusicPlayer::playPrevious() {
  const Song *prev = stack.getPreviousSong();
  if (prev == nullptr) {
    std::cout << "No previous songs" << std::endl;
    return; // Do nothing
  }
  // Only add current to forward if it's not null
  if (current != nullptr) {
    stack.addSongToForward(current);
  }
  current = prev;
  playSong();
}

void MusicPlayer::enableShuffle() {
  if (queue.getQueueSize() == 0) {
    std::cout << "Cannot enable shuffle: queue is empty" << std::endl;
    return;
  }
  isShuffleEn = true;
  shuffleMgr.enableShuffle(queue.getQueueList());
  std::cout << "Shuffle: ON" << std::endl;
}

bool MusicPlayer::isShuffleEnabled() const { return isShuffleEn; };

void MusicPlayer::disableShuffle() {
  isShuffleEn = false;
  std::cout << "Shuffle: OFF" << std::endl;
}

void MusicPlayer::selectAndPlaySong(int songID) {
  if (current != nullptr) {
    stack.addSongToHistory(current);
  }
  current = library.findSongByID(songID);
  playSong();
}

void MusicPlayer::selectAndAddSong(int songID) {
  addSongToQueue(library.findSongByID(songID));
}

void MusicPlayer::selectAndPlaySong(const std::string &title) {
  if (current != nullptr) {
    stack.addSongToHistory(current);
  }
  current = library.findSongByTitle(title);
  playSong();
}

void MusicPlayer::selectAndAddSong(const std::string &title) {
  addSongToQueue(library.findSongByTitle(title));
}

size_t MusicPlayer::addAlbumToQueue(const std::string &albumName) {
  int count = 0;
  for (const auto &song : library) {
    if (song.album == albumName) {
      const Song *songPtr = library.findSongByID(song.id);
      if (songPtr) {
        this->addSongToQueue(songPtr);
        count++;
      }
    }
  }
  return count;
}

void MusicPlayer::clearQueue() {
  queue.clearQueue();
  shuffleMgr.clear();   // Clear shuffle queue and history
  stack.clearForward(); // Clear forward stack
  isShuffleEn = false;
  std::cout << "Queue cleared" << std::endl;
}

PlaybackQueue MusicPlayer::generateSmartPlaylist(const Song *startSong,
                                                 size_t maxSize) {

  if (startSong == nullptr) {
    return {};
  }
  smartPlaylist.clearQueue();          // Clear smart playlist
  std::queue<std::string> searchQueue; // Search queue (by artist and album)
  std::unordered_set<const Song *> visited; // Contain pointers to song

  searchQueue.push(startSong->artist); // Criteria 1
  searchQueue.push(startSong->album);  // Criteria 2
  visited.insert(startSong);           // Mark current song as visited

  while (!searchQueue.empty() && (smartPlaylist.getQueueSize() < maxSize)) {
    std::string currentCriteria =
        searchQueue.front(); // Take a criteria into consideration
    searchQueue.pop();       // Release that criteria from queue

    for (const auto &song : library) {
      if (visited.find(&song) ==
          visited.end()) { // If song not already in "visited"
        if (song.artist == currentCriteria || song.album == currentCriteria) {
          smartPlaylist.addSong(&song); // Add that song to smart playlist
          visited.insert(&song);        // Mark as visited
        }
      }
    }
  }
  if (smartPlaylist.getQueueSize() == 0) {
    std::cout << "No songs in Smart Playlist" << std::endl;
    return {};
  }
  std::cout << "Added " << smartPlaylist.getQueueSize()
            << "songs in Smart Playlist" << std::endl;
  return smartPlaylist;
}

void MusicPlayer::applySmartPlaylist() {
  if (smartPlaylist.getQueueSize() == 0) {
    return;
  }
  isShuffleEn = false;   // turn OFF shuffle
  stack.clearForward();  // clear forward stack
  queue.clearQueue();    // clear the current queue
  queue = smartPlaylist; // copy smart playlist to current playing queue
  std::cout << "Smart Playlist applied" << std::endl;
}

const ShuffleManager &MusicPlayer::getShuffleManager() const {
  return shuffleMgr;
}

void MusicPlayer::clearHistory() {
  stack.clearHistory();
  std::cout << "History cleared" << std::endl;
}

void MusicPlayer::clearLibrary() {
  // Stop current playback
  engine.stop();
  current = nullptr;

  // Clear all queues
  queue.clearQueue();
  shuffleMgr.clear();
  stack.clearHistory();
  stack.clearForward();
  smartPlaylist.clearQueue();

  // Clear library and loaded folders
  library.clear();
  loadedFolders.clear();
  isShuffleEn = false;

  std::cout << "Library unloaded" << std::endl;
}

size_t MusicPlayer::addArtistToQueue(const std::string &artistName) {
  std::vector<const Song *> artistSongs = library.findSongByArtist(artistName);
  for (const Song *song : artistSongs) {
    addSongToQueue(song);
  }
  std::cout << "Added " << artistSongs.size()
            << " songs from artist: " << artistName << std::endl;
  return artistSongs.size();
}
