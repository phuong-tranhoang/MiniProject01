// HistoryManager.h
// Manages the playback history

#ifndef HISTORY_H
#define HISTORY_H

#include <stack>
#include <vector>
#include "Song.h"

class PlaybackHistory {
private:
    std::stack<const Song*> history;
    std::stack<const Song*> forward;

public:
    /**===================================================
     * 
     * Description: Construct Class with a stack of previous songs
     *
     * @param {none}
     * @returns {none}  
     */
    PlaybackHistory();
    /**===================================================
     * 
     * Description: Destruct Class with a stack of previous songs
     *
     * @param {none}
     * @returns {none}  
     */
    ~PlaybackHistory();
    /**===================================================
     * 
     * Description: Return last played song and remove 
     *
     * @param {none}
     * @returns {const Song*} - Pointer to last played song
     */
    const Song* getPreviousSong();
    /**===================================================
     * 
     * Description: Return forward song and remove
     *
     * @param {none}
     * @returns {const Song*} - Pointer to forward song in stack
     */
    const Song* getForwardSong();
    /**===================================================
     * 
     * Description: Get history list
     *
     * @param {none}
     * @returns {std::vector<const Song*} - list in vector
     */
    std::vector<const Song*> getHistoryList() const;
    /**===================================================
     * 
     * Description: Add song to history stack
     *
     * @param {Song*} song - Current song just has been played
     * @returns {none}
     */    
    void addSongToHistory(const Song* song);
    /**===================================================
     * 
     * Description: Add song to forward stack
     *
     * @param {Song*} song - Current song to play but got push out by "play previous"
     * @returns {none}
     */
    void addSongToForward(const Song* song);
    /**===================================================
     * 
     * Description: Clear history
     *
     * @param {}
     * @returns {none}
     */    
    void clearHistory();
    /**===================================================
     * 
     * Description: Clear forward queue
     *
     * @param {}
     * @returns {none}
     */    
    void clearForward();
    /**===================================================
     * 
     * Description: Return history stack size
     *
     * @param {size_t} - Size of stack
     * @returns {none}
     */
    size_t getHistorySize() const;
    /**===================================================
     * 
     * Description: Return forward stack size
     *
     * @param {size_t} - Size of stack
     * @returns {none}
     */
    size_t getForwardSize() const;
};

#endif