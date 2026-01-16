// MusicLibrary.cpp

#include "../include/MusicLibrary.h"
#include <iostream>

MusicLibrary::MusicLibrary() { songs.reserve(1000); }

MusicLibrary::~MusicLibrary() {}

const std::vector<Song> &MusicLibrary::getAllSongs() const { return songs; }

bool MusicLibrary::addSong(const Song &song) {
  bool reallocated = false;
  if (songs.size() >= songs.capacity()) {
    reallocated = true;
    std::cout << "Vector Reallocation" << std::endl;
  }

  songs.push_back(song);

  const Song *songPtr = &songs.back();

  // Update maps

  songIndexByID[song.id] = songPtr;
  songIndexByTitle[song.title] = songPtr;
  artistIndex[song.artist].push_back(songPtr);
  albumIndex[song.album].push_back(songPtr);
  return reallocated; //
}

std::vector<Song>::const_iterator MusicLibrary::begin() const {
  return songs.begin();
}

std::vector<Song>::const_iterator MusicLibrary::end() const {
  return songs.end();
}

size_t MusicLibrary::getSize() const { return songs.size(); }

void MusicLibrary::initIDMap() {
  for (int i = 0; i < songs.size(); ++i) {
    const Song *ptr = &songs[i];
    songIndexByID[ptr->id] = ptr;
  }
}

void MusicLibrary::initTitleMap() {
  for (int i = 0; i < songs.size(); ++i) {
    const Song *ptr = &songs[i];
    songIndexByTitle[ptr->title] = ptr;
  }
}

void MusicLibrary::initArtistMap() {
  for (int i = 0; i < songs.size(); ++i) {
    const Song *ptr = &songs[i];
    artistIndex[ptr->artist].push_back(ptr);
  }
}

const Song *MusicLibrary::findSongByID(int id) const {
  auto it = songIndexByID.find(id);

  if (it != songIndexByID.end()) {
    return it->second; // Accessing 'value' using ->second
  }
  return nullptr;
}

const Song *MusicLibrary::findSongByTitle(const std::string &title) {
  auto it = songIndexByTitle.find(title);

  if (it != songIndexByTitle.end()) {
    return it->second; // Accessing 'value' using ->second
  }
  return nullptr;
}

std::vector<const Song *>
MusicLibrary::findSongByArtist(const std::string &artist) {
  auto it = artistIndex.find(artist);

  if (it != artistIndex.end()) {
    return it->second; // Accessing 'value' using ->second
  }
  return {};
}

void MusicLibrary::clear() {
  songs.clear();
  songIndexByID.clear();
  songIndexByTitle.clear();
  artistIndex.clear();
  albumIndex.clear();
  Song::resetIdCounter(); // Reset ID counter for next library load
}

const std::unordered_map<std::string, std::vector<const Song *>> &
MusicLibrary::getArtistIndex() const {
  return artistIndex;
}

const std::map<std::string, std::vector<const Song *>> &
MusicLibrary::getAlbumIndex() const {
  return albumIndex;
}

std::vector<const Song *> MusicLibrary::getSortedSongs() const {
  std::vector<const Song *> sorted;
  // songIndexByTitle is a std::map (ordered by key)
  for (const auto &pair : songIndexByTitle) {
    sorted.push_back(pair.second);
  }
  return sorted;
}