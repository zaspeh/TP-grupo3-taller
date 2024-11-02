#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "weapon.h"
#include "equipment.h"

class PlayerState {
private:
    duck_t duck;
    Weapon* weapon;
    Armor armor;
    Helmet helmet;

    float verticalVelocity;  // Velocidad vertical para el salto
    const float gravity = -9.8;  // Valor de gravedad (ejemplo)
    const float jumpStrength = 15.0;  // Fuerza del salto

public:
    // Constructor por defecto
    PlayerState() : 
        weapon(nullptr), 
        armor(), 
        helmet(),
        verticalVelocity(0.0)
    {
        duck.pos = {0, 0};
        duck.isAlive = true;
        duck.health = 100;
    }


/* typedef struct {
    position_t pos; // Posición actual del pato en el nivel
    uint8_t id;                 // ID único del pato para identificar al jugador
    bool faceLeft;          // Dirección hacia la que mira el pato
    bool isJumping;         // Estado de salto del pato
    bool isDucking;         // Estado de estar tirado al piso
    bool isFalling;         // Si el jugador está en caída libre
    bool isFlaping;         // Si el jugador está en salto
    uint8_t health;             // Salud actual del pato
    bool isAlive;             // Estado de vida del pato (vivo o muerto)
    uint8_t score;              // Puntaje acumulado del pato
    uint8_t color;              // Color asignado al pato   
    weapon_t equipped_weapon; // Arma equipada por el pato
    armor_t equipped_armor;   // Armadura o casco equipado por el pato
} duck_t; */

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

    /* bool isCrouched() const { return duck.isCrouched; }
    void setCrouched() { duck.isCrouched = true; } */

    bool hasArmorEquipped() const { return armor.isEquipped(); }
    void setArmorEquipped() { armor.equip(); }    

    bool hasHelmetEquipped() const { return helmet.isEquipped(); }
    void setHelmetEquipped() { helmet.equip(); }

    uint8_t getHealth() const { return duck.health; }  
    void setHealth(uint8_t newHealth) { duck.health = newHealth; }

    uint8_t getFacingDirection() const { return duck.faceLeft; }
    void setFacingDirection(uint8_t direction) { duck.faceLeft = direction; }; 
    void move(int dx, int dy);

    /* int getScore() const;
    void setScore(int newScore);

    uint8_t getDuckColor() const;
    void setDuckColor(uint8_t color); */

    void takeDamage(uint8_t damage);

    void shoot();

    // Simulación de salto
    void jump() {
        if (!duck.isFalling) {
            verticalVelocity = jumpStrength;
            duck.isFalling = true;
        }
    }
    //void crouch() { duck.isCrouched = true; }

    void dropWeapon();
    void equipArmor() { armor.equip(); }
    void equipHelmet() { helmet.equip(); }


    // Actualizar posición vertical considerando física
    void updatePosition(float deltaTime) {
        if (duck.isFalling) {
            verticalVelocity += gravity * deltaTime;
            duck.pos.y += verticalVelocity * deltaTime;
            if (duck.pos.y <= 0) {  // Asumimos que y=0 es el suelo
                duck.pos.y = 0;
                verticalVelocity = 0;
                duck.isFalling = false;
            }
        }
    }
};

#endif // PLAYERSTATE_H


// Notas:
/*
Hacer una clase Weapon y luego utilizar herencia.

*/