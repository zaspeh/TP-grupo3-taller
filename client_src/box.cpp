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

/*
zaspeh@44931392:~/Escritorio/tp/TP-grupo3-taller$ ./client localhost 8080
Ingrese el ID de cliente: 0
ID inválido. Debe estar en el rango [0, 255].
ID inválido. Debe estar en el rango [0, 255].
Id inicializando en: 0
REceiver: 0
Sender: 0
Client ID: 0
Initialized ducks vector with 0 ducks.
Initialized ducks vector with 114 platforms.
Tamaño de spawns: 8
Creando nuevo spawn: 5
Creando nuevo spawn: 0
Creando nuevo spawn: 6
Creando nuevo spawn: 0
Creando nuevo spawn: 0
Creando nuevo spawn: 1
Creando nuevo spawn: 0
Creando nuevo spawn: 2
Unable to load image client_src/spawn/box1.png! SDL_image Error: Unsupported image format
Failed to load box texture.
Failed to load texture for box 0.
Failed to initialize game or load media.
^C^C^CTerminado (killed)

*/

bool Box::loadTexture() {
    std::string path = "client_src/spawn/box1.png";

/*
Unable to load image client_src/spawn/box1.png! SDL_image Error: Unsupported image format
Failed to load box texture from client_src/spawn/box1.png
SDL_image Error: Unsupported image format
Failed to load texture for box 0.
Failed to initialize game or load media.

*/

    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load box texture from " << path << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;  // Imprime el error detallado
        return false;
    }
    return true;
}


void Box::render() {
    if (boxState.is_destroyed) {
        return; 
    }

    scaleRect.x = boxState.pos.x;
    scaleRect.y = boxState.pos.y;
    
    texture->render(boxState.pos.x, boxState.pos.y, nullptr, &scaleRect, SDL_FLIP_NONE);
}

void Box::updateState(const box_t& newState) {
    boxState = newState;
}