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
  int duration; // [second]
  std::string filePath;
  // Constructor
  Song(int id, std::string t, std::string ar, std::string al, int d,
       std::string p)
      : id(getNextId()), title(t), artist(ar), album(al), duration(d),
        filePath(p) {}

  // Default constructor - uses unique ID
  Song() : id(getNextId()) {}

private:
  static int getNextId() {
    static std::atomic<int> id{0};
    return id++;
  };
};

#endif