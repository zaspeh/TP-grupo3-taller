#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ltexture.h"
#include <unordered_map> 
#include "../common_src/utils.h"
#include "../common_src/game_state.h"
#include <memory>
#include <map>

class Weapon{
private:
    std::unordered_map<uint8_t, std::unique_ptr<LTexture>> guns;
    weapon_t weaponState;
    std::map<int, int> weaponYOffsets;
public:
    Weapon(weapon_t weaponState, SDL_Renderer* renderer);
    void render(float x, float y, bool faceLeft);
    bool loadTexture();
    void updateState(const weapon_t& newWeaponState);
    int getType() const { return weaponState.type; }
    weapon_t getState() const { return weaponState; }
};


#endif