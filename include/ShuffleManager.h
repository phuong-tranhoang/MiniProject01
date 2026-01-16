// ShuffleManager.h
// Manages the shuffle list

#ifndef SHUFFLE_H
#define SHUFFLE_H

#include "PlaybackQueue.h"
#include "Song.h"
#include <unordered_set>

class ShuffleManager {
private:
  std::vector<const Song *> shuffleQueue;
  std::unordered_set<const Song *> shuffleHistory;
  int index = -1;

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
   * Description: Shuffle all songs in shuffleQueue (includes index 0) and clear
   * history
   *
   * @param {none}
   * @returns {none}
   */
  void shuffleAll();
  /**===================================================
   *
   * Description: Copy queue into shuffleQueue and shuffle all
   *
   * @param {std::list<const Song*>} queue - upcoming PlaybackQueue
   * @returns {none}
   */
  void enableShuffle(std::list<const Song *> queue);
  /**===================================================
   *
   * Description: Get the current song in shuffle_list
   *
   * @param {none}
   * @returns {const Song*} - Pointer to current song
   */
  //const Song *getCurrentSong();
  /**===================================================
   *
   * Description: Get the next song in shuffle_list
   *
   * @param {none}
   * @returns {const Song*} pointer to next song
   */
  const Song *getNextSong();
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
  void addSong(const Song *song);
    /**===================================================
   *
   * Description: Add album to the shuffle queue
   *
   * @param {const Song*} album - album name
   * @returns {none}
   */
  size_t addAlbumToQueue(const std::string&albumName, const MusicLibrary &library);
  /**===================================================
   *
   * Description: Get the shuffled queue for visual display
   *
   * @param {none}
   * @returns {const std::vector<const Song*>&} reference to shuffle queue
   */
  const std::vector<const Song *> &getShuffleList() const;
  /**===================================================
   *
   * Description: Get current index in shuffle queue
   *
   * @param {none}
   * @returns {size_t} current index
   */
  size_t getCurrentIndex() const;
  /**===================================================
   *
   * Description: Clear shuffle queue and reset
   *
   * @param {none}
   * @returns {none}
   */
  void clear();
};

#endif