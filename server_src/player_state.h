#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "weapon.h"
#include "equipment.h"

#include <iostream>

class PlayerState {
private:
    duck_t duck;
    Weapon* weapon;
    Armor armor;
    Helmet helmet;

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
        duck.isAlive = true;
        duck.health = 100;
        duck.isJumping = false;
        duck.isFalling = false;  // Aseguramos que inicie en false
        duck.id = clientID;
    }

    duck_t getState() {
        
        return duck;
    }

    // Getters y Setters para cada atributo
    position_t getPosition() const { return duck.pos; }
    void setPosition(const position_t& newPosition)  { duck.pos = newPosition; }

    uint8_t getWeapon() const { return weapon->getId(); } // ahora en weapon
    void pickWeapon(Weapon* newWeapon);

    uint8_t getAmmo() const { return weapon->getAmmo(); } // ahora en weapon
    void setAmmo(uint8_t newAmmo) { weapon->setAmmo(newAmmo); }
    void reload(uint8_t newAmmo);

    bool isAlive() const { return duck.isAlive; }
    void setAlive() { duck.isAlive = !duck.isAlive; }

    bool isFalling() const { return duck.isFalling; }
    void setFalling() { duck.isFalling = true; }

    bool isCrouched() const { return duck.isDucking; } // por què isDucking?
    void setCrouched(bool isCrouched) { duck.isDucking = isCrouched; }

    bool hasArmorEquipped() const { return armor.isEquipped(); }
    void setArmorEquipped() { armor.equip(); }    

    bool hasHelmetEquipped() const { return helmet.isEquipped(); }
    void setHelmetEquipped() { helmet.equip(); }

    uint8_t getHealth() const { return duck.health; }  
    void setHealth(uint8_t newHealth) { duck.health = newHealth; }

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

    void shoot();

    //void crouch() { duck.isCrouched = true; }

    void dropWeapon();
    void equipArmor() { armor.equip(); }
    void equipHelmet() { helmet.equip(); }


    void updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms);

    bool checkPlatformBelow(platform_t* platforms, uint8_t numPlatforms, float newY);

    void forcePlatformState();

    void jump();
};

#endif // PLAYERSTATE_H


// Notas:
/*
Hacer una clase Weapon y luego utilizar herencia.

*/