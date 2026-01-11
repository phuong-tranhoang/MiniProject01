// MusicPlayer.h
// The Main Player

#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "Song.h"
#include "MusicLibrary.h"
#include "PlaybackQueue.h"
#include "ShuffleManager.h"
#include "PlaybackHistory.h"
#include "AudioEngine.h"

class MusicPlayer {
private:
    MusicLibrary library;
    PlaybackQueue queue;
    ShuffleManager shuffleMgr;
    PlaybackHistory stack;

    bool isShuffleEn = false;
    const Song* current = nullptr;
    PlaybackQueue smartPlaylist;

public:
    AudioEngine engine;

    /**===================================================
     * 
     * Description: Construct MusicPlayer class
     *
     * @param {none}
     * @returns {none}  
     */
    MusicPlayer();
    /**===================================================
     * 
     * Description: Destruct MusicPlayer class
     *
     * @param {none}
     * @returns {none}  
     */
    ~MusicPlayer();
    /**===================================================
     * 
     * Description: Load songs into library
     *
     * @param {const std::string} path - folder path include songs
     * @returns {none}  
     */
    void loadLibrary(const std::string& path);
    /**===================================================
     * 
     * Description: Get library
     *
     * @param {const std::vector<Song>&} reference of library
     * @returns {none}  
     */
    const MusicLibrary& getLibrary() const;
    /**===================================================
     * 
     * Description: Load songs into library
     *
     * @param {const std::string} path - folder path include songs
     * @returns {none}  
     */
    void loadMetadata(const std::string& filePath, Song& newSong);
    /**===================================================
     * 
     * Description: Return library size
     *
     * @param {none}
     * @returns {none}  
     */
    size_t getLibrarySize();
    /**===================================================
     * 
     * Description: Get current song
     *
     * @param {none}
     * @returns {const Song*} Current song
     */
    const Song* getCurrentSong();
    /**===================================================
     * 
     * Description: Get upcoming queue
     *
     * @param {PlaybackQueue&} reference of queue
     * @returns {none}  
     */
    const PlaybackQueue& getQueueManager() const;
    /**===================================================
     * 
     * Description: Get history
     *
     * @param {PlaybackQueue&} reference of history
     * @returns {none}  
     */
    const PlaybackHistory& getHistoryManager() const;
    /**===================================================
     * 
     * Description: Add song into queue
     *
     * @param {const Song*} song - Song to be added
     * @returns {none}  
     */
    void addSongToQueue(const Song* song);
    /**===================================================
     * 
     * Description: Get the number of songs in queue
     *
     * @param {size}
     * @returns {none}  
     */
    size_t getQueueSize();
    /**===================================================
     * 
     * Description: Play current song
     *
     * @param {none}
     * @returns {none}  
     */
    void playSong();
    /**===================================================
     * 
     * Description: Choose and play a song
     *
     * @param {const Song*} song - song to be played
     * @returns {none}  
     */
    void chooseAndPlaySong(const Song* song);
    /**===================================================
     * 
     * Description: Play next song
     *
     * @param {none}
     * @returns {none}  
     */
    void playNext();
    /**===================================================
     * 
     * Description: Play previous song and put current song in forward
     *
     * @param {none}
     * @returns {none}  
     */
    void playPrevious();
    /**===================================================
     * 
     * Description: Enable shuffle mode and create shuffle list from queue
     *
     * @param {none}
     * @returns {none}  
     */
    void enableShuffle();
    /**===================================================
     * 
     * Description: Disable shuffle mode
     *
     * @param {none}
     * @returns {none}  
     */
    void disableShuffle();
    /**===================================================
     * 
     * Description: Return state of shuffle mode
     *
     * @param {none}
     * @returns {bool} true: shuffle ON --- false: shuffle OFF  
     */
    bool isShuffleEnabled() const;
    /**===================================================
     * 
     * Description: Find and play song by ID (immediately)
     *
     * @param {int} songID - Song ID
     * @returns {none}  
     */
    void selectAndPlaySong(int songID);
    /**===================================================
     * 
     * Description: Find and add song by ID (to queue)
     *
     * @param {int} songID - Song ID
     * @returns {none}  
     */
    void selectAndAddSong(int songID);
    /**===================================================
     * 
     * Description: Find and play song by title (immediately)
     *
     * @param {const std::string&} title - Song title
     * @returns {none}  
     */
    void selectAndPlaySong(const std::string& title);
    /**===================================================
     * 
     * Description: Find and add song by title (to queue)
     *
     * @param {const std::string&} title - Song ID
     * @returns {none}  
     */
    void selectAndAddSong(const std::string& title);
    /**===================================================
     * 
     * Description: Find and add songs from album (to queue)
     *
     * @param {const std::string&} albumName - Album name
     * @returns {none}  
     */
    void addAlbumToQueue(const std::string& albumName);
    /**===================================================
     * 
     * Description: Clear the queue and reset shuffle mode
     *
     * @param {const std::string&} albumName - Album name
     * @returns {none}  
     */
    void clearQueue();
    /**===================================================
     * 
     * Description: Generate a smart playlist based on a song
     *
     * @param {const Song*} startSong
     * @returns {none}  
     */
    PlaybackQueue generateSmartPlaylist(const Song* startSong, size_t maxSize);
    /**===================================================
     * 
     * Description: Clear current queue and apply the smart playlist
     *
     * @param {none} startSong
     * @returns {none}  
     */
    void applySmartPlaylist(); 
};

#endif