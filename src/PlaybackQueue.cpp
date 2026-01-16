// PlaybackQueue

#include "../include/PlaybackQueue.h"
#include <iostream>

PlaybackQueue::PlaybackQueue() {}

PlaybackQueue::~PlaybackQueue() {}

std::list<const Song *> PlaybackQueue::getQueueList() const { return queue; }

void PlaybackQueue::addSong(const Song *song) { queue.push_back(song); }

void PlaybackQueue::removeSong(const Song *targetSong) {
  queue.remove_if(
      [targetSong](const Song *ptrSong) { return ptrSong == targetSong; });
}

void PlaybackQueue::clearQueue() {
  if (queue.empty()) {
    return;
  }
  queue.clear();
}

size_t PlaybackQueue::getQueueSize() const { return queue.size(); }

// const Song *PlaybackQueue::getCurrentSong() const {
//   if (queue.empty()) {
//     std::cout << "Queue is empty." << std::endl;
//     return nullptr;
//   }
//   return queue.front();
// }

const Song *PlaybackQueue::getNextSong() {
  if (queue.empty()) {
    return nullptr;
  }
  const Song *next = queue.front();
  queue.pop_front();
  return next;
}

size_t PlaybackQueue::addAlbumToQueue(const std::string &albumName, const MusicLibrary &library) {
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
