// MusicLibrary.h
// Manages the storage and searching/indexing of songs

#ifndef MUSIC_LIB_H
#define MUSIC_LIB_H

#include "Song.h"
#include <map>
#include <unordered_map>
#include <vector>

class MusicLibrary {
private:
  std::vector<Song> songs;
  std::unordered_map<int, const Song *> songIndexByID;
  std::map<std::string, const Song *> songIndexByTitle;
  std::unordered_map<std::string, std::vector<const Song *>> artistIndex;
  std::map<std::string, std::vector<const Song*>> albumIndex;

public:
  /**===================================================
   *
   * Description: Construct Class with a vector with 1000 songs reserved
   *
   * @param {none}
   * @returns {none}
   */
  MusicLibrary();
  /**===================================================
   *
   * Description: Destruct Class
   *
   * @param {none}
   * @returns {none}
   */
  ~MusicLibrary();
  /**===================================================
   *
   * Description: Get all songs from library
   *
   * @param {none}
   * @returns {const std::vector<Song>&} reference of all the songs
   */
  const std::vector<Song> &getAllSongs() const;
  /**===================================================
   *
   * Description: Add Song to Library and update map
   *
   * @param {const Song&} song - Song to add to library
   * @returns {bool} true if reallocated - false if not reallocated
   */
  bool addSong(const Song &song);
  /**===================================================
   *
   * Description: Return begin of Class (MusicLibrary)
   *
   * @param {none}
   * @returns {iterator} begin of songs vector
   */
  std::vector<Song>::const_iterator begin() const;
  /**===================================================
   *
   * Description: Return end of Class (MusicLibrary)
   *
   * @param {none}
   * @returns {iterator} end of songs vector
   */
  std::vector<Song>::const_iterator end() const;
  /**===================================================
   *
   * Description: Return size of Class (MusicLibrary)
   *
   * @param {none}
   * @returns {size_t} number of songs in library
   */
  size_t getSize() const;
  /**===================================================
   *
   * Description: Initialize and populate ID hashmap
   * - Use only when vector is allocated
   * @param {none}
   * @returns {none}
   * @note key: ID
   * @note value: pointer to song (in library)
   */
  void initIDMap();
  /**===================================================
   *
   * Description: Initialize and populate Title map (auto balancing BST)
   * - Use only when vector is allocated
   * @param {none}
   * @returns {none}
   * @note key: title
   * @note value: pointer to song (in library)
   */
  void initTitleMap();
  /**===================================================
   *
   * Description: Initialize and populate Artist hashmap
   * - Use only when vector is allocated
   * @param {none}
   * @returns {none}
   * @note key: artist
   * @note value: vector pointers to songs
   */
  void initArtistMap();
  /**===================================================
   *
   * Description: Find song by ID using hashmap
   *
   * @param {int} id - ID for search
   * @returns {const Song*} - Pointer to song in library
   * @note return nullptr if not found
   */
  const Song *findSongByID(int id) const;
  /**===================================================
   *
   * Description: Find song by Title using map
   *
   * @param {string} title - Title for search
   * @returns {const Song*} - Pointer to song in library
   * @note return nullptr if not found
   */
  const Song *findSongByTitle(const std::string &title);
  /**===================================================
   *
   * Description: Find song by Artist using map
   *
   * @param {string} artist - Artist for search
   * @returns {const vector<Song*.} - Pointer to song in library
   * @note return nullptr if not found
   */
  std::vector<const Song *> findSongByArtist(const std::string &title);
  /**===================================================
   *
   * Description: Clear all songs from library
   *
   * @param {none}
   * @returns {none}
   */
  void clear();
  /**===================================================
   *
   * Description: Get the artist index map directly
   *
   * @param {none}
   * @returns {const unordered_map<string, vector<Song*>>&} reference to artist
   * index
   */
  const std::unordered_map<std::string, std::vector<const Song *>>& getArtistIndex() const;
      /**===================================================
   *
   * Description: Find song by Album using map
   *
   * @param {string} album - album name
   * @returns {const Song*}
   * @note return nullptr if not found
   */
  const std::map<std::string, std::vector<const Song*>>& getAlbumIndex() const;

  /**===================================================
   *
   * Description: Get all songs sorted by title (A-Z)
   *
   * @param {none}
   * @returns {vector<const Song*>} sorted vector of song pointers
   */
  std::vector<const Song *> getSortedSongs() const;
};

#endif