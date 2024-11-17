#ifndef BOX_H
#define BOX_H

#include <memory>
#include "ltexture.h"
#include "../common_src/game_state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Box {
private:
    box_t boxState;
    SDL_Renderer* gRenderer;
    std::unique_ptr<LTexture> texture;
    SDL_Rect scaleRect;
    
    static const int BOX_WIDTH = 40;
    static const int BOX_HEIGHT = 40;

public:
    Box(box_t boxState, SDL_Renderer* renderer);
    ~Box() = default;

    bool loadTexture();
    void render();
    void updateState(const box_t& newState);
    box_t getState() const { return boxState; }
};

#endif // BOX_H