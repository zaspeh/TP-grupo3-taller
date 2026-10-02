#ifndef BANANA_H
#define BANANA_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ltexture.h"
#include "camera.h"
#include "../common_src/game_state.h"

class Banana {
private:
    SDL_Renderer* renderer;
    std::unique_ptr<LTexture> texture;
    position_t position;
    SDL_Rect scaleRect;

public:
    explicit Banana(SDL_Renderer* renderer);
    ~Banana() = default;

    // Prevenir copia
    Banana(const Banana&) = delete;
    Banana& operator=(const Banana&) = delete;

    // Permitir movimiento
    Banana(Banana&&) = default;
    Banana& operator=(Banana&&) = default;

    bool loadTexture();
    void render(const Camera& camera, float zoom);
    void updatePosition(const position_t& pos);
};

#endif // BANANA_H