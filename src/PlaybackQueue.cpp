// PlaybackQueue

#include <iostream>
#include "../include/PlaybackQueue.h"


PlaybackQueue::PlaybackQueue() {}

PlaybackQueue::~PlaybackQueue() {}

std::list<const Song*> PlaybackQueue::getQueueList() const {
    return queue;
}

void PlaybackQueue::addSong(const Song* song) {
    queue.push_back(song);
}


void PlaybackQueue::removeSong(int songID) {
    queue.remove_if([songID](const Song* ptrSong) {
        return ptrSong->id == songID;
    });
}

void PlaybackQueue::clearQueue() {
    if (queue.empty()){
        return;
    }
    queue.clear();
}

size_t PlaybackQueue::getQueueSize() const {
    return queue.size();
}

const Song* PlaybackQueue::getCurrentSong() const {
    if (queue.empty()) {
        std::cout << "Queue is empty." << std::endl;
        return nullptr;
    }
    return queue.front();
}

const Song* PlaybackQueue::getNextSong() {
    if (queue.empty()) {
        return nullptr; 
    }
    queue.pop_front();
    return getCurrentSong();
}

size_t PlaybackQueue::addAlbumToQueue(const std::string& albumName, const MusicLibrary& library) {
    int count = 0;
    for (auto song : library) { 
        if (song.album == albumName) {
            this->addSong(&song);
            count++;
        }
    }
    return count;
}