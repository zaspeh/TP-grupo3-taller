#ifndef ARMOR_H
#define ARMOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ltexture.h"
#include <unordered_map> 
#include "../common_src/utils.h"
#include "../common_src/game_state.h"
#include "camera.h"
#include <memory>

class Armor {
protected:

    armor_t nullArmor = {{0, 0}, NULL_ARMOR};

    std::unordered_map<uint8_t, std::unique_ptr<LTexture>> armors;  
    armor_t helmetState;  
    armor_t chestplateState;  

    void renderArmorPiece(uint8_t armorType, armor_t& armorState, int x, int y, bool faceLeft, const Camera& camera, float zoom);  

public:
    Armor(armor_t armorState, SDL_Renderer* renderer);  
    void render(int x, int y, bool faceLeft, uint8_t type, const Camera& camera, float zoom); 
    bool loadTexture();  
    void updateState(armor_t arm); 
    int getType();
    armor_t getState();
};

#endif
