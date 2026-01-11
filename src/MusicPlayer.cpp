// MusicPlayer.cpp

#include "../include/MusicPlayer.h"
#include <iostream>
#include <filesystem>
#include <queue>
#include <taglib/fileref.h>
#include <taglib/tag.h>

namespace fs = std::filesystem;

MusicPlayer::MusicPlayer(){
    engine.init();
}

MusicPlayer::~MusicPlayer(){}

void MusicPlayer::loadLibrary(const std::string& path){
    if (!fs::exists(path)) {
        std::cerr << "Error: Path does not exist -> " << path << std::endl;
        return; 
    }

    int count = 0;

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.path().extension() == ".mp3" || entry.path().extension() == ".wav") {
            Song newSong;
            newSong.filePath = entry.path().string(); // Add filepath to Song struct!

            // Default metadata
            newSong.title = entry.path().stem().string();
            newSong.artist = "Unknown Artist";
            newSong.album = "Unknown Album";

            loadMetadata(newSong.filePath, newSong);
            
            library.addSong(newSong);
            count++;
        }
    }
    std::cout << "Loaded: " << count << " songs from " << path << std::endl;
}

const MusicLibrary& MusicPlayer::getLibrary() const {
    return library;
}

void MusicPlayer::loadMetadata(const std::string& filePath, Song& newSong) {
    TagLib::FileRef f(filePath.c_str());
    
    if(!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        newSong.title = tag->title().to8Bit(true);   // Convert String to std::string
        newSong.artist = tag->artist().to8Bit(true);
        newSong.album = tag->album().to8Bit(true);
        newSong.duration = f.audioProperties()->lengthInSeconds();
    }
}

size_t MusicPlayer::getLibrarySize(){
    return library.getSize();
}

const Song* MusicPlayer::getCurrentSong(){
    return current;
}

const PlaybackQueue& MusicPlayer::getQueueManager() const { return queue; }

const PlaybackHistory& MusicPlayer::getHistoryManager() const { return stack; }

void MusicPlayer::addSongToQueue(const Song* song){
    queue.addSong(song);
    if (isShuffleEn == true){
        shuffleMgr.addSong(song);
    }
}

size_t MusicPlayer::getQueueSize(){
    return queue.getQueueSize();
}

void MusicPlayer::playSong(){
    if (current == nullptr){
        std::cout << "No song to play" << std::endl;
        return;
    }
    engine.playFile(current->filePath);     // play the song
}

void MusicPlayer::chooseAndPlaySong(const Song* song){
    if (current != nullptr){
        stack.addSongToHistory(current);        // push current song into history
    }
    current = song;                             // play the newly chosen song
    playSong();
}

void MusicPlayer::playNext(){
    if (current != nullptr){
        stack.addSongToHistory(current);        // add current song to history
    }

    const Song* next = nullptr;
    
    if (stack.getForwardSize() != 0){
        next = stack.getForwardSong();
    }
    else if (isShuffleEn){
        next = shuffleMgr.getNextSong();
    }
    else {
        next = queue.getNextSong();
    }
    if (next != nullptr){
        current = next;
        playSong();
    }
    else {
        std::cout<<"No Songs in Queue"<<std::endl;
    }
}

void MusicPlayer::playPrevious(){
    const Song* prev = stack.getPreviousSong();
    if (prev == nullptr){
        std::cout<<"No previous songs"<<std::endl;
        return;                       // Do nothing
    }                                   
    stack.addSongToForward(current);  // Put current song in forward
    current = prev;
    playSong();
}

void MusicPlayer::enableShuffle(){
    isShuffleEn = true;
    shuffleMgr.enableShuffle(queue.getQueueList());
    std::cout << "Shuffle: ON" << std::endl;
}

bool MusicPlayer::isShuffleEnabled() const{
    return isShuffleEn;
};

void MusicPlayer::disableShuffle(){
    isShuffleEn = false;
    std::cout << "Shuffle: OFF" << std::endl;
}


void MusicPlayer::selectAndPlaySong(int songID){
    if (current != nullptr){
        stack.addSongToHistory(current);
    }
    current = library.findSongByID(songID);
    playSong();
}

void MusicPlayer::selectAndAddSong(int songID){
    addSongToQueue(library.findSongByID(songID));
}

void MusicPlayer::selectAndPlaySong(const std::string& title){
    if (current != nullptr){
        stack.addSongToHistory(current);
    }
    current = library.findSongByTitle(title);
    playSong();
}

void MusicPlayer::selectAndAddSong(const std::string& title){
    addSongToQueue(library.findSongByTitle(title));
}

void MusicPlayer::addAlbumToQueue(const std::string& albumName){
    size_t count = queue.addAlbumToQueue(albumName, library);
    std::cout << "Added " << count << " songs from album: " << albumName << std::endl;
}

void MusicPlayer::clearQueue(){
    queue.clearQueue();
    
    isShuffleEn = false;
    std::cout << "Queue cleared...Smart Shuffle OFF" << std::endl;
}

PlaybackQueue MusicPlayer::generateSmartPlaylist(const Song* startSong, size_t maxSize){
    smartPlaylist.clearQueue();                     // Clear smart playlist
    std::queue<std::string> searchQueue;            // Search queue (by artist and album)
    std::unordered_set<const Song*> visited;        // Contain pointers to song

    searchQueue.push(startSong->artist);            // Criteria 1
    searchQueue.push(startSong->album);             // Criteria 2
    visited.insert(startSong);                      // Mark current song as visited

    while (!searchQueue.empty() && (smartPlaylist.getQueueSize() < maxSize) ) {
        std::string currentCriteria = searchQueue.front();      // Take a criteria into consideration
        searchQueue.pop();                                      // Release that criteria from queue

        for (const auto& song : library) {
            if (visited.find(&song) == visited.end()) {         // If song not already in "visited"
                if (song.artist == currentCriteria || song.album == currentCriteria) {
                    smartPlaylist.addSong(&song);               // Add that song to smart playlist
                    visited.insert(&song);                      // Mark as visited 
                }
            }
        }
    }
    if (smartPlaylist.getQueueSize() == 0){
        std::cout << "No songs in Smart Playlist" << std::endl;
        return{};
    }
    std::cout << "Added " << smartPlaylist.getQueueSize() << "songs in Smart Playlist" << std::endl;
    return smartPlaylist;
}

void MusicPlayer::applySmartPlaylist(){
    if (smartPlaylist.getQueueSize() == 0){
        return;
    }
    isShuffleEn = false;    // turn OFF shuffle
    stack.clearForward();   // clear forward stack
    queue.clearQueue();     // clear the current queue
    queue = smartPlaylist;  // copy smart playlist to current playing queue
    std::cout << "Smart Playlist applied" << std::endl;
}
