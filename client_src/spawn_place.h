#ifndef SPAWNPLACE_H
#define SPAWNPLACE_H

#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include <SDL2/SDL.h>
#include <memory>
#include "ltexture.h"
#include <SDL2/SDL_image.h>

class SpawnPlace {
public:
    // Constructor
    SpawnPlace(const spawn_place_t& spawnData, SDL_Renderer* renderer);

    // Destructor
    ~SpawnPlace();

    // Cargar las texturas del SpawnPlace
    bool loadTexture();

    // Renderizar el SpawnPlace en el nivel
    void render();

    // Obtener la posición del spawn
    position_t getPosition() const;

    // Obtener el estado de activación
    bool isActive() const;

private:
    spawn_place_t spawnData;  // Datos de la estructura de spawn
    std::unique_ptr<LTexture> spawnTexture;  // Textura del spawn
    SDL_Renderer* renderer;  // Renderizador de SDL
};

#endif // SPAWNPLACE_H
