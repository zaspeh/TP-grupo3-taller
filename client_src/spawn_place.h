#ifndef SPAWNPLACE_H
#define SPAWNPLACE_H

#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include <SDL2/SDL.h>
#include <memory>
#include "ltexture.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include "weapon.h"
#include "camera.h"
#include "armor.h"

class SpawnPlace {
public:

    SpawnPlace(const spawn_place_t& spawnData, SDL_Renderer* renderer);

    ~SpawnPlace();

    bool loadTexture();
    void render(const Camera& camera, float zoom);
    void updateState(const spawn_place_t& newState);
    position_t getPosition() const;


    bool isActive() const;

private:
    spawn_place_t spawnData;  
    std::unique_ptr<LTexture> spawnTexture;  
    SDL_Renderer* renderer;  
    std::unique_ptr<Weapon> weapon;
    std::unique_ptr<Armor> armor;
    std::unique_ptr<LTexture> chestplateTexture;
    std::unique_ptr<LTexture> helmetTexture;
};

#endif // SPAWNPLACE_H
