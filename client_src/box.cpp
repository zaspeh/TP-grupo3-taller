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
            //path += "box0.png";
            break;
        default:
            std::cerr << "Caja no cargada" << std::endl;
            break;
    }

    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load box texture from " << path << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;  // Imprime el error detallado
        return false;
    }
    return true;
}


void Box::render() {
    scaleRect.x = boxState.pos.x;
    scaleRect.y = boxState.pos.y;
    
    texture->render(boxState.pos.x, boxState.pos.y, nullptr, &scaleRect, SDL_FLIP_NONE);
}

void Box::updateState(const box_t& newState) {
    boxState = newState;
    loadTexture();
}