// LoopSimulation.cpp

#include "../include/LoopSimulation.h"
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

LoopSimulation::LoopSimulation(){}

LoopSimulation::~LoopSimulation(){}

void LoopSimulation::autoPlay(){
    if (player.getCurrentSong() != nullptr && player.engine.checkFinished()){
        player.playNext();
    }
}

void LoopSimulation::simLoop(){
    player.loadLibrary(folderPath);         // Load library
    
    // Simulation Loop
    while (true) {
            autoPlay();

        // 2. Handle User Input (non-blocking if possible)
        // In a console app, std::cin is blocking, so you might need a separate thread
        // or just accept that input pauses the check loop.
        
        // ... Input handling code ...
        // If user selects song -> player.selectAndPlay(...) -> audio.playFile(...)
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep to save CPU
    }
}