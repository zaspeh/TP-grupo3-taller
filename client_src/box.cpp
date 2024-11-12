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
    std::string path;
    // segun la vida de la caja deberia cambiar el path
    //path = "client_src/spawn/box1.png";

    //if (!texture->loadFromFile(path)) {
    //    std::cerr << "Failed to load box texture." << std::endl;
    //    return false;
    //}
    return true;
}

void Box::render() {
    if (boxState.is_destroyed) {
        return; // No renderizar cajas destruidas
    }

    scaleRect.x = boxState.pos.x;
    scaleRect.y = boxState.pos.y;
    
    texture->render(boxState.pos.x, boxState.pos.y, nullptr, &scaleRect, SDL_FLIP_NONE);
}

void Box::updateState(const box_t& newState) {
    boxState = newState;
}