#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include "ltexture.h"
#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include "camera.h"

class Projectile {
public:
    Projectile(projectile_t projectileState, SDL_Renderer* renderer);
    ~Projectile() = default;

    bool loadTexture();
    void updateState(const projectile_t& newState);
    void render(const Camera& camera, float zoom);
    bool getState() const { return projectileState.is_active; }

private:
    projectile_t projectileState;
    SDL_Renderer* gRenderer;
    std::unique_ptr<LTexture> texture;
    SDL_Rect scaleRect;
};

#endif // PROJECTILE_H
