// AudioEngine.cpp

#include "../include/AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() : songFinished(false) {}

AudioEngine::~AudioEngine() { ma_engine_uninit(&engine); }

void AudioEngine::init() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        // Handle error
        return;
    }
    isInitialized = true;
}

void AudioEngine::playFile(const std::string& filepath) {
    if (!isInitialized) return;

    // Unload previous sound if exists
    ma_sound_uninit(&sound);

    // Load new file (mp3/wav)
    if (ma_sound_init_from_file(&engine, filepath.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS) {
        std::cout << "Failed to load file: " << filepath << std::endl;
        return;
    }

    ma_sound_start(&sound);
}

bool AudioEngine::isPlaying() {
    return ma_sound_is_playing(&sound);
}

void AudioEngine::setPaused(bool paused) {
        if (!isInitialized) return;
        if (paused) {
            ma_sound_stop(&sound); // Stop processing PCM data
        } else {
            ma_sound_start(&sound); // Resume from cursor
        }
    }

    // Seek to a specific time (in Seconds)
    void AudioEngine::seekTo(float seconds) {
        if (!isInitialized) return;
        // Convert seconds to PCM frames (Sample Rate usually 44100 or 48000)
        ma_uint64 frameIndex = (ma_uint64)(seconds * 48000); 
        ma_sound_seek_to_pcm_frame(&sound, frameIndex);
    }

    float AudioEngine::getCurrentTime() {
        if (!isInitialized) return 0.0f;
        float time = ma_sound_get_time_in_milliseconds(&sound)/1000.0f;
        return time;
    }

    float AudioEngine::getTotalDuration() {
        if (!isInitialized) return 0.0f;
        float length;
        ma_sound_get_length_in_seconds(&sound, &length);
        return length;
    }

bool AudioEngine::checkFinished() {
    if (isInitialized && ma_sound_at_end(&sound)) {
        return true;
    }
    return false;
}

void AudioEngine::stop() { ma_sound_stop(&sound); }
