#include "platform.h"
#include <iostream>

Platform::Platform(position_t pos, SDL_Renderer* renderer)
: pos(pos)
{
    try {
        platformTexture = std::make_unique<LTexture>(renderer);
    }catch (const std::bad_alloc& e){
        std::cerr << "Failed to create platform: " << e.what() << std::endl;
    }
}

bool Platform::loadTexture(){
    return platformTexture->loadFromFile("client_src/platform.png");
}

void Platform::updateState(const position_t& newPosState){
    pos = newPosState;
}

void Platform::render(){
    SDL_Rect* scaleRect = new SDL_Rect{0, 0, 0, 0};
    scaleRect->h = platformTexture->getHeight();
    scaleRect->w = platformTexture->getWidth(); 
    scaleRect->x = platformTexture->getHeight();
    scaleRect->y = platformTexture->getWidth(); 
    platformTexture->render(pos.x+32, pos.y+32, NULL, scaleRect, SDL_FLIP_NONE);
}
