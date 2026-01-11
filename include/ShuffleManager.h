// ShuffleManager.h
// Manages the shuffle list

#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <unordered_set>
#include "Song.h"
#include "PlaybackQueue.h"

class ShuffleManager {
private:
    std::vector<const Song*> shuffleQueue;
    std::unordered_set<const Song*> shuffleHistory;
    int index = 0;
public:
    /**===================================================
     * 
     * Description: Construct Class with a vector of songs in queue
     *
     * @param {none}
     * @returns {none}  
     */
    ShuffleManager();
    /**===================================================
     * 
     * Description: Destruct Shuffle class
     *
     * @param {none}
     * @returns {none}  
     */
    ~ShuffleManager();
    /**===================================================
     * 
     * Description: Shuffle all songs in shuffleQueue (includes index 0) and clear history
     *
     * @param {none}
     * @returns {none}
     */
    void shuffleAll();
    /**===================================================
     * 
     * Description: Shuffle all songs in shuffleQueue (except index 0) and clear history
     *
     * @param {none}
     * @returns {none}
     */
    void shuffle();
    /**===================================================
     * 
     * Description: Copy current queue into a shuffleQueue, shuffle
     *
     * @param {std::list<const Song*>} - queue: current PlaybackQueue
     * @returns {none}
     */
    void enableShuffle(std::list<const Song*> queue);
    /**===================================================
     * 
     * Description: Get the current song in shuffle_list
     *
     * @param {none}
     * @returns {const Song*} - Pointer to current song
     */
    const Song* getCurrentSong();
    /**===================================================
     * 
     * Description: Get the next song in shuffle_list
     *
     * @param {none}
     * @returns {const Song*} pointer to next song
     */
    const Song* getNextSong();
    /**===================================================
     * 
     * Description: Auto loop if queue ended
     *
     * @param {none}
     * @returns {none}
     */
    void checkAutoLoop();
    /**===================================================
     * 
     * Description: Add song to the end of shuffle queue
     *
     * @param {const Song*} song - Song to be added
     * @returns {none}
     */
    void addSong(const Song* song);
};

#endif