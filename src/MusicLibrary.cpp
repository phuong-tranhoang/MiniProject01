// MusicLibrary.cpp

#include "../include/MusicLibrary.h"

MusicLibrary::MusicLibrary() {
    songs.reserve(1000);
}

MusicLibrary::~MusicLibrary() {}

bool MusicLibrary::addSong(const Song& song) {
    const Song* songPtr = &songs.back();

    if (songs.size()>=songs.capacity()){
        songs.push_back(song);
        // ReInit entire map if vector is allocated (>1000 songs)
        initIDMap();
        initTitleMap();
        initArtistMap();
        return true;       // return false if reallocated
    }
    
    songs.push_back(song);

    // Update maps
    
    songIndexByID[song.id] = songPtr;
    songIndexByTitle[song.title] = songPtr;
    artistIndex[song.artist].push_back(songPtr);
    return false;            // return true if not reallocated
}

std::vector<Song>::const_iterator MusicLibrary::begin() const {
    return songs.begin();
}

std::vector<Song>::const_iterator MusicLibrary::end() const {
    return songs.end();
}

size_t MusicLibrary::getSize() const {
    return songs.size();
}

void MusicLibrary::initIDMap(){
    for (int i = 0; i < songs.size(); ++i){
        const Song* ptr = &songs[i];
        songIndexByID[ptr->id] = ptr ;
    }
}

void MusicLibrary::initTitleMap(){
    for (int i = 0; i < songs.size(); ++i){
        const Song* ptr = &songs[i];
        songIndexByTitle[ptr->title] = ptr;
    }
}

void MusicLibrary::initArtistMap(){
    for (int i = 0; i < songs.size(); ++i){
        const Song* ptr = &songs[i];
        artistIndex[ptr->artist].push_back(ptr);
    }
}

const Song* MusicLibrary::findSongByID(int id) {
    auto it = songIndexByID.find(id);

    if (it != songIndexByID.end()) {
        return it->second;     // Accessing 'value' using ->second
    }
    return nullptr;
}

const Song* MusicLibrary::findSongByTitle(const std::string& title) {
    auto it = songIndexByTitle.find(title);

    if (it != songIndexByTitle.end()) {
        return it->second;     // Accessing 'value' using ->second
    }
    return nullptr;
}

std::vector<const Song*> MusicLibrary::findSongByArtist(const std::string& artist) {
    auto it = artistIndex.find(artist);

    if (it != artistIndex.end()) {
        return it->second;     // Accessing 'value' using ->second
    }
    return {};
}