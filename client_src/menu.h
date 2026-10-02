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
#include "../common_src/config_manager.h"

class Menu {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font{nullptr, TTF_CloseFont};
    std::unique_ptr<LTexture> backgroundTexture;
    std::unique_ptr<LTexture> logoTexture;
    std::unique_ptr<LTexture> yellowDuck;
    std::unique_ptr<LTexture> orangeDuck;
    std::unique_ptr<LTexture> grayDuck;
    std::unique_ptr<LTexture> whiteDuck;
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
    YAML::Node config = ConfigManager::getInstance();
    
    const int WINDOW_WIDTH = config["general"]["level_width"].as<int>();
    const int WINDOW_HEIGHT = config["general"]["level_height"].as<int>();
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