// En client_src/music.h
#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>
#include <string>

class Music {
private:
    Mix_Music* backgroundMusic;
    bool isInitialized;
    int volume;

public:
    Music();
    ~Music();

    bool init();
    bool loadMusic(const std::string& path);
    void play(int loops = -1);  // -1 significa loop infinito
    void stop();
    void pause();
    void resume();
    void setVolume(int volume);  
    bool isPlaying() const;
    void fadeIn(int ms);
    void fadeOut(int ms);
    void cleanup();
};

#endif

