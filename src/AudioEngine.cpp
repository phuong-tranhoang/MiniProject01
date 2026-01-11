// AudioEngine.cpp

#define MINIAUDIO_IMPLEMENTATION 


#include "../include/AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() {
    ma_engine_config config = ma_engine_config_init();
    config.periodSizeInMilliseconds = 250; // Increase buffer to 250ms to prevent jitter

    if (ma_engine_init(&config, &engine) == MA_SUCCESS) {
        isInitialized = true;
    }
}

AudioEngine::~AudioEngine() { 
    if (isSoundLoaded) ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}

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
    if (isSoundLoaded) {
            ma_sound_stop(&sound);
            ma_sound_uninit(&sound);
            isSoundLoaded = false;
    }

    ma_uint32 flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC;

    // Load new file (mp3/wav)
    if (ma_sound_init_from_file(&engine, filepath.c_str(), flags, NULL, NULL, &sound) == MA_SUCCESS) {
        ma_sound_start(&sound);
        isSoundLoaded = true;
    }
    isSoundLoaded = true;
    ma_sound_start(&sound);
}

bool AudioEngine::isPlaying() {
    return isSoundLoaded && ma_sound_is_playing(&sound);
}

void AudioEngine::togglePause() {
    if (!isSoundLoaded) return;

    if (ma_sound_is_playing(&sound)) {
        ma_sound_stop(&sound);
    } else {
        if (ma_sound_at_end(&sound)) {
            ma_sound_seek_to_pcm_frame(&sound, 0);
        }
        ma_sound_start(&sound);
    }
}

void AudioEngine::seekTo(float seconds) {
    if (!isSoundLoaded) return;

    bool wasPlaying = ma_sound_is_playing(&sound);
    
    if (wasPlaying) {
            ma_sound_stop(&sound);
        }
        
    ma_uint32 sampleRate;
    ma_sound_get_data_format(&sound, NULL, NULL, &sampleRate, NULL, 0);     // Usually 44100 or 48000
    ma_uint64 targetFrame = (ma_uint64)(seconds * sampleRate);

    ma_sound_seek_to_pcm_frame(&sound, targetFrame);
    
    // Resume
    if (wasPlaying) {
        ma_sound_start(&sound);
    }
}

float AudioEngine::getCurrentTime() {
    if (!isSoundLoaded) return 0.0f;
    float time = ma_sound_get_time_in_milliseconds(&sound)/1000.0f;
    return time;
}

float AudioEngine::getTotalDuration() {
    if (!isSoundLoaded) return 1.0f;
    float length = 0.0f;
    ma_sound_get_length_in_seconds(&sound, &length);
    return (length > 0) ? length : 1.0f;
}

bool AudioEngine::isFinished() {
    return (isSoundLoaded && ma_sound_at_end(&sound));
}

void AudioEngine::stop() {ma_sound_stop(&sound);}