#ifndef MENU_H_
#define MENU_H_

#include "ltexture.h"
#include <SDL2/SDL.h>
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
    TTF_Font* font;
    LTexture* backgroundTexture;
    LTexture* logoTexture;
    LTexture* yellowDuck;
    LTexture* orangeDuck;
    LTexture* grayDuck;
    LTexture* whiteDuck;
    SDL_Rect buttonPlayRect;
    SDL_Rect buttonYellowDuckRect;
    SDL_Rect buttonGrayDuckRect;
    SDL_Rect buttonOrangeDuckRect;
    SDL_Rect buttonWhiteDuckRect;
    bool isRunning;
    bool gameStarted;
    bool isButtonPlayHovered;
    bool isButtonPlayPressed;
    Uint32 blinkTimer;
    bool showText;
    bool closed;
    int chosenColor;
    
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
    bool isButtonHovered(SDL_Rect button, int mouseX, int mouseY);
    bool running() const { return isRunning; }
    int chosenDuckColor() const { return chosenColor; }
    bool wasClosed() const { return closed; }
    void stop() { isRunning = false; }
};

#endif