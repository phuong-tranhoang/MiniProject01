// Song.h
#ifndef SONG_H
#define SONG_H

#include <atomic>
#include <string>

struct Song {
  int id;
  std::string title;
  std::string artist;
  std::string album;
  size_t duration; // [second]
  std::string filePath;

  // Constructor
  Song(int id, std::string t, std::string ar, std::string al, size_t d,
       std::string p)
      : id(getNextId()), title(t), artist(ar), album(al), duration(d),
        filePath(p) {}

  // Default constructor - uses unique ID
  Song() : id(getNextId()) {}

  // Reset ID counter (call when unloading library)
  static void resetIdCounter() { idCounter = 0; }

private:
  static inline std::atomic<int> idCounter{0};

  static int getNextId() { return idCounter++; }
};

#endif