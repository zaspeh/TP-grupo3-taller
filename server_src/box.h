#ifndef BOX_H
#define BOX_H
#include "../common_src/game_state.h"
#include "../common_src/utils.h"




class Box {
    private:
    box_t boxState;
    armor_t armorState;
    weapon_t weaponState;
    bool is_broken;

public:
    Box(box_t boxState, armor_t armorState, weapon_t weaponState) : boxState(boxState), armorState(armorState), weaponState(weaponState), is_broken(false) {};
    box_t getBoxState() const { return boxState; }
    void setBoxState(box_t& newState) { boxState = newState; }
    armor_t getArmorState() const { return armorState; }
    weapon_t getWeaponState() const { return weaponState; }
    bool isBroken() const { return is_broken; }
    void breakBox() { is_broken = true; }
};

#endif // BOX_H