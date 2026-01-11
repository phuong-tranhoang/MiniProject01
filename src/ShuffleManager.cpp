// ShuffleManager.cpp

#include "../include/ShuffleManager.h"
#include <algorithm>
#include <iostream>
#include <random>

ShuffleManager::ShuffleManager() {}

ShuffleManager::~ShuffleManager() {}

void ShuffleManager::shuffleAll() {
  std::random_device seed; // random seed
  std::mt19937 g(seed());  // generator

  std::shuffle(shuffleQueue.begin(), shuffleQueue.end(),
               g); // begin() is the current song (playing) -> also shuffle

  shuffleHistory.clear(); // reset
  index = 0;              // reset
}

void ShuffleManager::shuffle() {
  std::random_device seed; // random seed
  std::mt19937 g(seed());  // generator

  std::shuffle(shuffleQueue.begin() + 1, shuffleQueue.end(),
               g); // begin() is the current song (playing) -> no shuffle

  shuffleHistory.clear(); // reset
  index = 0;              // reset
}

void ShuffleManager::enableShuffle(std::list<const Song *> queue) {
  // Clear previous shuffle session first
  shuffleQueue.clear();
  shuffleHistory.clear();
  index = 0;

  // Copy from queue to shuffle vector
  for (const Song *song : queue) {
    shuffleQueue.push_back(song);
  }
  shuffle();
  if (!shuffleQueue.empty()) {
    shuffleHistory.insert(shuffleQueue[0]);
  }
}

const Song *ShuffleManager::getCurrentSong() { return shuffleQueue[index]; }

const Song *ShuffleManager::getNextSong() {
  if (shuffleQueue.empty()) {
    std::cout << "No songs in queue" << std::endl;
    return nullptr;
  }

  do {
    checkAutoLoop();
  } while (shuffleHistory.insert(shuffleQueue[index]).second == false);

  return getCurrentSong();
}

void ShuffleManager::checkAutoLoop() {
  index++;
  if (index >= shuffleQueue.size()) {
    shuffleAll(); // Reshuffle and reset index to 0 for repeat
  }
}

void ShuffleManager::addSong(const Song *song) { shuffleQueue.push_back(song); }

const std::vector<const Song *> &ShuffleManager::getShuffleList() const {
  return shuffleQueue;
}

size_t ShuffleManager::getCurrentIndex() const { return index; }

void ShuffleManager::clear() {
  shuffleQueue.clear();
  shuffleHistory.clear();
  index = 0;
}
