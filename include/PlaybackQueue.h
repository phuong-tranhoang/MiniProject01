// PlaybackQueue.h
// Manages the user playlist (current, next, previous).// Manages the storage and searching/indexing of songs

#ifndef PLAYBACK_QUEUE_H
#define PLAYBACK_QUEUE_H

#include <list>
#include "Song.h"
#include "MusicLibrary.h"

class PlaybackQueue {
private:
    std::list<const Song*> queue;
public:
    /**===================================================
     * 
     * Description: Construct Class with a list contains songs from queue
     *
     * @param {none}
     * @returns {none}  
     */
    PlaybackQueue();
    /**===================================================
     * 
     * Description: Destruct Class
     *
     * @param {none}
     * @returns {none}  
     */
    ~PlaybackQueue();
    /**===================================================
     *
     * Description: Get queue list
     *
     * @param {none}
     * @returns {std::list<const Song*>} - queue list   
     */
    std::list<const Song*> getQueueList() const ;
    /**===================================================
     * 
     * Description: Add song to playback queue
     *
     * @param {const Song*} song - pointer of song (in library) to add to queue
     * @returns {none}  
     */
    void addSong(const Song* song);
    /**===================================================
     * 
     * Description: Remove song (pointer) from queue using ID
     *
     * @param {const Song*} targetSong - song to remove
     * @returns {none}  
     */
    void removeSong(const Song* targetSong);
    /**===================================================
     * 
     * Description: Clear current queue
     *
     * @param {none}
     * @returns {none}  
     */
    void clearQueue();
    /**===================================================
     * 
     * Description: Get size of queue
     *
     * @param {none}
     * @returns {size_t} - number of song in queue
     * @note
     */
    size_t getQueueSize() const;
    /**===================================================
     * 
     * Description: Get current song in queue
     *
     * @param {none}
     * @returns {const Song*} - pointer to current song in queue
     * @note return nullptr if queue empty
     */
    //const Song* getCurrentSong() const; 
    /**===================================================
     * 
     * Description: Get the next song in queue and remove the current song
     *
     * @param {none}
     * @returns {const Song*} - Pointer to next song in list 
     * @note do nothing if queue empty 
     */
    const Song* getNextSong();
    /**===================================================
     * 
     * Description: Add songs in an album to queue
     *
     * @param {string} albumName - name of album
     * @param {MusicLibrary&} library - music library that contains album
     * @returns {size_t} - number of songs (in album) added  
     */   
    size_t addAlbumToQueue(const std::string& albumName, const MusicLibrary& library);
};

#endif