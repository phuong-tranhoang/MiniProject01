// ShuffleManager.cpp

#include <algorithm>
#include <random>
#include <iostream>
#include "../include/ShuffleManager.h"

ShuffleManager::ShuffleManager(){}

ShuffleManager::~ShuffleManager(){}

void ShuffleManager::shuffleAll(){
    std::random_device seed;    // random seed
    std::mt19937 g(seed());     // generator

    std::shuffle(shuffleQueue.begin(), shuffleQueue.end(), g);    // begin() is the current song (playing) -> also shuffle

    shuffleHistory.clear();     // reset
    index = 0;                  // reset
}

void ShuffleManager::shuffle(){
    std::random_device seed;    // random seed
    std::mt19937 g(seed());     // generator

    std::shuffle(shuffleQueue.begin()+1, shuffleQueue.end(), g);    // begin() is the current song (playing) -> no shuffle

    shuffleHistory.clear();     // reset
    index = 0;                  // reset
}

void ShuffleManager::enableShuffle(std::list<const Song*> queue ){
    for (const Song* song : queue){
        shuffleQueue.push_back(song);
    }
        shuffle();
        if (!shuffleQueue.empty()){
            shuffleHistory.insert(shuffleQueue[0]);
        }
}

const Song* ShuffleManager::getCurrentSong(){
    return shuffleQueue[index];
}

const Song* ShuffleManager::getNextSong(){
    if (shuffleQueue.empty()){
        return nullptr;
        std::cout << "No songs in queue" << std::endl;
    }

    do {
        checkAutoLoop();
    } while (shuffleHistory.insert(shuffleQueue[index]).second == false);

    return getCurrentSong();
}

void ShuffleManager::checkAutoLoop(){
    index ++;
    if (index > shuffleQueue.size()){
        shuffleAll();
    }
}

void ShuffleManager::addSong(const Song* song){
    shuffleQueue.push_back(song);
}
