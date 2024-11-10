#include "platform.h"
#include "../common_src/utils.h"
#include <iostream>

Platform::Platform(platform_t platform, SDL_Renderer* renderer)
: platform(platform)
{
    try {
        platformTexture = std::make_unique<LTexture>(renderer);
    }catch (const std::bad_alloc& e){
        std::cerr << "Failed to create platform: " << e.what() << std::endl;
    }
}

bool Platform::loadTexture(){
    bool loaded = false;
    if(platform.type == GRASS_PLATFORM){
        loaded = platformTexture->loadFromFile("client_src/platform.png");
    }else{
        loaded = platformTexture->loadFromFile("client_src/platformdirt.png");
    }
    return loaded;
}

void Platform::updateState(const platform_t& newPlatformState){
    platform = newPlatformState;
}

void Platform::render(){
    SDL_Rect* scaleRect = new SDL_Rect{0, 0, 0, 0};
    scaleRect->h = platformTexture->getHeight();
    scaleRect->w = platformTexture->getWidth(); 
    scaleRect->x = platformTexture->getHeight();
    scaleRect->y = platformTexture->getWidth(); 
    platformTexture->render(platform.pos.x+32, platform.pos.y+32, NULL, scaleRect, SDL_FLIP_NONE);
}
