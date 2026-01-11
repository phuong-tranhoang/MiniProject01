// PlaybackHistory.cpp

#include "../include/PlaybackHistory.h"

PlaybackHistory::PlaybackHistory(){}

PlaybackHistory::~PlaybackHistory(){}

const Song* PlaybackHistory::getPreviousSong(){
    if (history.empty()){
        return nullptr;
    }
    const Song* prev = history.top();
    history.pop();
    return prev;
}

const Song* PlaybackHistory::getForwardSong(){
    if (forward.empty()){
        return nullptr;
    }
    const Song* next = forward.top();
    forward.pop();
    return next;
}

void PlaybackHistory::addSongToHistory(const Song* song){
    history.push(song);
}

void PlaybackHistory::addSongToForward(const Song* song){
    forward.push(song);
}

void PlaybackHistory::clearHistory(){
    while (!history.empty()){
        history.pop();
    }
}

void PlaybackHistory::clearForward(){
    while (!forward.empty()){
        forward.pop();
    }
}

size_t PlaybackHistory::getHistorySize() const {
    return history.size();
}

size_t PlaybackHistory::getForwardSize() const {
    return forward.size();
}