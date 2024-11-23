// box.cpp
#include "box.h"
#include <iostream>

Box::Box(box_t boxState, SDL_Renderer* renderer)
    : boxState(boxState),
      gRenderer(renderer),
      texture(std::make_unique<LTexture>(renderer))
{
    scaleRect.w = BOX_WIDTH;
    scaleRect.h = BOX_HEIGHT;
}

bool Box::loadTexture() {
    if (boxState.health == 0) { 
        return true;
    }
    
    std::string path = "client_src/spawn/";

    switch (boxState.health) {
        case 4:
            path += "box1.png";
            break;
        case 3: 
            path += "box2.png";
            break;
        case 2: 
            path += "box3.png";
            break;
        case 1:
            path += "box4.png";
            break;
        case 0:
            std::cout << "Caja sin vida: " << path << std::endl;
            break;
        default:
            std::cerr << "Caja no cargada" << std::endl;
            break;
    }

    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load box texture from " << path << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;  
        return false;
    }
    return true;
}


void Box::render(const Camera& camera, float zoom) {
    scaleRect.x = boxState.pos.x;
    scaleRect.y = boxState.pos.y;

    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    texture->render(destRect.x, destRect.y, nullptr, &destRect, SDL_FLIP_NONE);
}

void Box::updateState(const box_t& newState) {
    boxState = newState;
    if (boxState.health > 0)
        loadTexture();
}