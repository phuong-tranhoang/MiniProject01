// ShuffleManager.cpp

#include "../include/ShuffleManager.h"
#include <algorithm>
#include <iostream>
#include <random>

ShuffleManager::ShuffleManager() {}

ShuffleManager::~ShuffleManager() {}

void ShuffleManager::shuffleAll() {
  std::random_device seed;
  std::mt19937 g(seed());
  std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), g);
  shuffleHistory.clear();
}

void ShuffleManager::enableShuffle(std::list<const Song *> queue) {
  // Clear previous shuffle session
  shuffleQueue.clear();
  shuffleHistory.clear();
  index = -1;

  // Copy all upcoming queue songs (current song is managed separately by
  // MusicPlayer)
  for (const Song *song : queue) {
    shuffleQueue.push_back(song);
  }

  // Shuffle all songs
  shuffleAll();
}

//const Song *ShuffleManager::getCurrentSong() { return shuffleQueue[index]; }

const Song *ShuffleManager::getNextSong() {
  if (shuffleQueue.empty()) {
    std::cout << "No songs in queue" << std::endl;
    return nullptr;
  }
  do {
    index++;
    if (index >= shuffleQueue.size()) {
      shuffleAll();
      index = 0;
    }
  }
  while (shuffleHistory.insert(shuffleQueue[index]).second == false);
  return shuffleQueue[index];
}

void ShuffleManager::addSong(const Song *song) { shuffleQueue.push_back(song); }

const std::vector<const Song *> &ShuffleManager::getShuffleList() const {
  return shuffleQueue;
}

size_t ShuffleManager::addAlbumToQueue(const std::string &albumName, const MusicLibrary &library) {
  int count = 0;
  for (const auto &song : library) {
    if (song.album == albumName) {

      const Song *songPtr = library.findSongByID(song.id);
      if (songPtr) {
        this->addSong(songPtr);
        count++;
      }
    }
  }
  return count;
}

size_t ShuffleManager::getCurrentIndex() const { return index; }

void ShuffleManager::clear() {
  shuffleQueue.clear();
  shuffleHistory.clear();
  index = -1;
}
