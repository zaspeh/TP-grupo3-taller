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
    } else {
        loaded = platformTexture->loadFromFile("client_src/platformdirt.png");
    }
    return loaded;
}

void Platform::updateState(const platform_t& newPlatformState){
    platform = newPlatformState;
}

void Platform::render(const Camera& camera, float zoom){
    SDL_Rect scaleRect = {0, 0, 0, 0};
    scaleRect.h = platformTexture->getHeight();
    scaleRect.w = platformTexture->getWidth(); 
    scaleRect.x = platform.pos.x;
    scaleRect.y = platform.pos.y; 

    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);

    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    platformTexture->render(destRect.x+(32*zoom), destRect.y+(32*zoom), NULL, &destRect, SDL_FLIP_NONE);
}
