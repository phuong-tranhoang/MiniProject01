// Song.h
#ifndef SONG_H
#define SONG_H

#include <string>
#include <atomic>

struct Song {
    int id;
    std::string title;
    std::string artist;
    std::string album;
    int duration;   // [second]
    std::string filePath;
    // Constructor 
    Song(int id, std::string t, std::string ar, std::string al, int d, std::string p)
        : id(getNextId()), title(t), artist(ar), album(al), duration(d), filePath(p) {}
    
    // Default constructor 
    Song() : id(0) {} 
private:
    static int getNextId(){
        static std::atomic<int> id{0};
        return id++;
    };
};

#endif