#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "weapon.h"
#include "equipment.h"
#include "../common_src/game_state.h"

#include <iostream>



class PlayerState {
private:
    weapon_t nullWeapon = {{0, 0}, NULL_WEAPON, 0};
    armor_t nullArmor = {{0, 0}, NULL_ARMOR};
    duck_t duck;
    Weapon* weapon;
    Armor armor;
    Helmet helmet;
    bool infinitAmmo = false;
    bool isOnGround = false;
    float verticalVelocity;  // Velocidad vertical para el salto
    //const float gravity = -9.8;  // Valor de gravedad (ejemplo)
    //const float jumpStrength = 15.0;  // Fuerza del salto
    const float gravity = 3000.0f;  // Gravedad positiva (hacia abajo)
    const float jumpStrength = -800.0f;  // Fuerza del salto negativa (hacia arriba)
    const float groundLevel = 100.0f;  // Ejemplo de nivel del suelo

public:
    // Constructor por defecto
    PlayerState(uint8_t clientID, int x, int y) : 
        weapon(nullptr), 
        verticalVelocity(0.0)
    {
        duck.pos = {x, y};
        duck.id = clientID;
        duck.faceLeft = false;
        duck.isJumping = false;
        duck.isDucking = false;
        duck.isFalling = false;  // Aseguramos que inicie en false
        duck.isFlaping = false;  // Aseguramos que inicie en false
        duck.health = 1;
        duck.isAlive = true;
        duck.score = 0;
        duck.color = 0;
        duck.equipped_weapon = nullWeapon;
        duck.helmet = nullArmor;
        duck.chestplate = nullArmor;
        infinitAmmo = false;
    }

    duck_t getState() {
        return duck;
    }
    void resetPlayer(duck_t newDuck, int x , int y) {
        std::cout << "Score del pato antes de ser modificado " << static_cast<int>(duck.score) << std::endl; 
        duck = newDuck;
        std::cout << "Score del pato después de ser modificado " << static_cast<int>(duck.score) << std::endl; 
        duck.pos = {x, y};
        duck.isAlive = true;
        duck.isJumping = false;
        duck.isFalling = false;   
        duck.isFlaping = false;
        duck.equipped_weapon.type = NULL_WEAPON;
        duck.chestplate.type = NULL_ARMOR;
        duck.helmet.type = NULL_ARMOR;
    }

    // Getters y Setters para cada atributo
    position_t getPosition() const { return duck.pos; }
    void setPosition(const position_t& newPosition)  { duck.pos = newPosition; }

    Weapon* getWeapon() const { return weapon; }
    uint8_t getWeaponType() const { return weapon->getId(); } // ahora en weapon
    weapon_t pickWeapon(Weapon* newWeapon);

    void updateWeapon();

    uint8_t getAmmo() const { return weapon->getAmmo(); } // ahora en weapon
    void setAmmo(uint8_t newAmmo) { weapon->setAmmo(newAmmo); }
    // player->setInfiniteAmmo(!player->isInfiniteAmmo());

    bool isInfiniteAmmo() const { return infinitAmmo; }
    void setInfiniteAmmo(bool infiniteAmmo) { infinitAmmo = infiniteAmmo; }

    bool isAlive()  { return duck.isAlive; }
    void setAlive() { duck.isAlive = !duck.isAlive; }

    bool isFalling() const { return duck.isFalling; }
    void setFalling() { duck.isFalling = true; }

    bool isCrouched() const { return duck.isDucking; } // por què isDucking?
    void setCrouched(bool isCrouched) { duck.isDucking = isCrouched; }

    bool hasArmorEquipped() const { return armor.isEquipped(); }
    void setArmorEquipped(armor_t armr) { 
        duck.chestplate = armr;
        armor.unequip();
        if (duck.chestplate.type != NULL_ARMOR)
            armor.equip(); 
        }    

    bool hasHelmetEquipped() const { return helmet.isEquipped(); }
    void setHelmetEquipped(armor_t hmt) { 
        duck.helmet = hmt;
        helmet.unequip();
        if (duck.helmet.type != NULL_ARMOR)
            helmet.equip(); 
        }

    uint8_t getFacingDirection() const { return duck.faceLeft; }
    void setFacingDirection(uint8_t direction) { duck.faceLeft = direction; }; 
    void move(int dx, int dy, platform_t* plat, uint8_t numPlats);

    bool doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x);

    bool doNotCollideY(platform_t* plat, uint8_t numPlats, int new_y);

    /* int getScore() const;
    void setScore(int newScore);

    uint8_t getDuckColor() const;
    void setDuckColor(uint8_t color); */

    void takeDamage(uint8_t damage);

    bool shoot();

    //void crouch() { duck.isCrouched = true; }

    weapon_t dropWeapon();
    void equipArmor() { armor.equip(); }
    void equipHelmet() { helmet.equip(); }


    void updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms, position_t* explotions, uint8_t numExplotions);

    bool checkPlatformBelow(platform_t* platforms, uint8_t numPlatforms, float newY);

    void forcePlatformState();

    void jump();
};

#endif // PLAYERSTATE_H

