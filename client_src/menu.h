#ifndef MENU_H_
#define MENU_H_

#include "ltexture.h"
#include <SDL2/SDL.h>
#include <memory>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../common_src/utils.h"
#include "ltexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Menu {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font{nullptr, TTF_CloseFont};
    std::unique_ptr<LTexture> backgroundTexture;
    std::unique_ptr<LTexture> logoTexture;
    /* TTF_Font* font;
    LTexture* backgroundTexture;
    LTexture* logoTexture; */
    SDL_Rect buttonRect;
    bool isRunning;
    bool gameStarted;
    bool isButtonHovered;
    Uint32 blinkTimer;
    bool showText;
    
    const int WINDOW_WIDTH = LEVEL_WIDTH;
    const int WINDOW_HEIGHT = LEVEL_HEIGHT;
    const int BUTTON_WIDTH = 200;
    const int BUTTON_HEIGHT = 50;
    const int BUTTON_TOP_MARGIN = 80;
    const Uint32 BLINK_INTERVAL = 500;

public:
    Menu();
    ~Menu();
    bool init(SDL_Window* gWindow, SDL_Renderer* gRenderer);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const { return isRunning; }
    void stop() { isRunning = false; }
};

#endif