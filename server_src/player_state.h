#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "weapon.h"
#include "equipment.h"
#include "../common_src/game_state.h"
#include <iostream>
#include <memory>

class PlayerState {
private:
    weapon_t nullWeapon = {{0, 0}, NULL_WEAPON, 0};
    armor_t nullArmor = {{0, 0}, NULL_ARMOR};
    duck_t duck;
    std::shared_ptr<Weapon> weapon;
    Armor armor;
    Helmet helmet;
    bool infinitAmmo = false;
    bool isOnGround = false;
    float verticalVelocity; 
    const float gravity = 3000.0f;  
    const float jumpStrength = -800.0f; 
    const float groundLevel = 100.0f; 
    bool isSlipping;
    float slipDistance;
    static constexpr float SLIP_SPEED = 650.0f;
    int preFace;
    float horizontalVelocity; 
    void applyRecoil(float recoilForce, platform_t* platforms, uint8_t numPlatforms);

public:
    PlayerState(uint8_t clientID, int x, int y);

    duck_t getState() { return duck; }
    
    void resetPlayer(duck_t newDuck, int x , int y);

    position_t getPosition() const { return duck.pos; }

    void setPosition(const position_t& newPosition)  { duck.pos = newPosition; }

    void setColor(uint8_t color)  { duck.color = color; }

    std::shared_ptr<Weapon> getWeapon() { return weapon; }

    uint8_t getWeaponType() const { return weapon->getId(); } 

    weapon_t pickWeapon(std::shared_ptr<Weapon> newWeapon);

    void updateWeapon(float deltaTime, level_t& level);

    uint8_t getAmmo() const { return weapon->getAmmo(); }

    void setAmmo(uint8_t newAmmo) { weapon->setAmmo(newAmmo); }

    bool isInfiniteAmmo() const { return infinitAmmo; }

    void setInfiniteAmmo(bool infiniteAmmo) { infinitAmmo = infiniteAmmo; }

    bool isAlive()  { return duck.isAlive; }

    void setAlive() { duck.isAlive = !duck.isAlive; }

    bool isFalling() const { return duck.isFalling; }

    void setFalling() { duck.isFalling = true; }

    bool isCrouched() const { return duck.isDucking; } 

    void setCrouched(bool isCrouched) { duck.isDucking = isCrouched; }

    bool hasArmorEquipped() const { return armor.isEquipped(); }

    void setArmorEquipped(armor_t armr);

    bool hasHelmetEquipped() const { return helmet.isEquipped(); }

    void setHelmetEquipped(armor_t hmt);

    uint8_t getFacingDirection() const { return duck.faceLeft; }

    void setFacingDirection(uint8_t direction) { duck.faceLeft = direction; }; 

    void move(int dx, int dy, platform_t* plat, uint8_t numPlats);

    bool doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x);

    /* int getScore() const;
    void setScore(int newScore);*/

    void takeDamage(uint8_t damage);

    bool shoot(platform_t* platforms, uint8_t numPlatforms);

    weapon_t dropWeapon();

    void equipArmor() { armor.equip(); }

    void equipHelmet() { helmet.equip(); }

    void updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms, position_t* explotions, uint8_t numExplotions);

    void jump();

    bool checkBananaCollision(position_t bananaPos);
};

#endif // PLAYERSTATE_H

