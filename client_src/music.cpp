
#include "music.h"
#include <iostream>

Music::Music() : backgroundMusic(nullptr), isInitialized(false), volume(MIX_MAX_VOLUME) {}

Music::~Music() {
    cleanup();
}

bool Music::init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer no pudo inicializarse! Error: " << Mix_GetError() << std::endl;
        return false;
    }
    isInitialized = true;
    return true;
}

bool Music::loadMusic(const std::string& path) {
    if (!isInitialized) {
        if (!init()) {
            return false;
        }
    }

    cleanup();  // Limpiamos cualquier música anterior

    backgroundMusic = Mix_LoadMUS(path.c_str());
    if (backgroundMusic == nullptr) {
        std::cerr << "¡No se pudo cargar la música! Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void Music::play(int loops) {
    if (backgroundMusic && !isPlaying()) {
        Mix_PlayMusic(backgroundMusic, loops);
        Mix_VolumeMusic(volume);
    }
}

void Music::stop() {
    if (isPlaying()) {
        Mix_HaltMusic();
    }
}

void Music::pause() {
    Mix_PauseMusic();
}

void Music::resume() {
    Mix_ResumeMusic();
}

void Music::setVolume(int newVolume) {
    volume = std::max(0, std::min(newVolume, MIX_MAX_VOLUME));
    Mix_VolumeMusic(volume);
}

bool Music::isPlaying() const {
    return Mix_PlayingMusic() != 0;
}

void Music::fadeIn(int ms) {
    if (backgroundMusic) {
        Mix_FadeInMusic(backgroundMusic, -1, ms);
    }
}

void Music::fadeOut(int ms) {
    Mix_FadeOutMusic(ms);
}

void Music::cleanup() {
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    if (isInitialized) {
        Mix_CloseAudio();
        isInitialized = false;
    }
}