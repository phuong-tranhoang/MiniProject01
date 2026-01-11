// AudioEngine.h
// Engine used to play the actual audio file

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include "miniaudio-master/miniaudio.h"
#include <string>
#include <atomic>

class AudioEngine {
private:
    ma_engine engine;
    ma_sound sound;
    bool isInitialized = false;
    bool isSoundLoaded = false;
    
    // Atomic flag to signal the main thread that a song finished
    std::atomic<bool> songFinished; 

public:
    /**===================================================
     * 
     * Description: Construct Class with songFinished = false
     *
     * @param {none}
     * @returns {none}  
     */
    AudioEngine();
    /**===================================================
     * 
     * Description: Destruct Class and uninit engine
     *
     * @param {none}
     * @returns {none}  
     */
    ~AudioEngine();
    /**===================================================
     * 
     * Description: Initialize audio engine
     *
     * @param {none}
     * @returns {none}  
     */
    void init();
    /**===================================================
     * 
     * Description: Play audio file ()
     *
     * @param {std::string&} filepath - Filepath to .mp3/.wav file
     * @returns {none}  
     */
    void playFile(const std::string& filepath);
    /**===================================================
     * 
     * Description: Check if audio file playing
     *
     * @param {none}
     * @returns {bool} true if playing, false if not playing  
     */
    bool isPlaying();
    /**===================================================
     * 
     * Description: Pause/Play audio
     *
     * @param {none}
     * @returns {none}
     */
    void togglePause();
    /**===================================================
     * 
     * Description: Go to timestamp
     *
     * @param {float} seconds - timestamp to go to
     * @returns {none} 
     */
    void seekTo(float seconds);
    /**===================================================
     * 
     * Description: Get the current timestamp
     *
     * @param {none}
     * @returns {float} current timestamp 
     */
    float getCurrentTime();
    /**===================================================
     * 
     * Description: Get duration of song
     *
     * @param {none}
     * @returns {float} duration
     */
    float getTotalDuration();
    /**===================================================
     * 
     * Description: Check if audio file finished
     *
     * @param {none}
     * @returns {bool} true if finished, false if not finished  
     */
    bool isFinished();
    /**===================================================
     * 
     * Description: Stop playing
     *
     * @param {none}
     * @returns {none}
     */
    void stop();
};

#endif