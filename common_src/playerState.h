#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "position.h"
#include "weapon.h"
#include "equipment.h"
#include "helmet.h"

class PlayerState {
private:
    Position position;    // Posición del jugador en el mapa
    bool alive;           // Estado de vida del jugador
    bool falling;         // Si el jugador está en caída libre
    bool flapping;        // Si el jugador está aleteando para caer más despacio
    bool crouched;        // Si el jugador está tirado en el piso simulando estar muerto
    uint8_t health;       // Cantidad de "vidas" o "golpes" que el jugador puede recibir
    uint8_t facingDirection;  // Dirección en la que el jugador está mirando (-1 izquierda, 1 derecha)
    uint8_t score;            // Puntuación del jugador
    uint8_t duckColor;    // Color asignado al pato para identificar al jugador
    Weapon weapon;
    Armor armor;
    Helmet helmet;

    float verticalVelocity;  // Velocidad vertical para el salto
    const float gravity = -9.8;  // Valor de gravedad (ejemplo)
    const float jumpStrength = 15.0;  // Fuerza del salto

public:
    // Constructor por defecto
    PlayerState() : 
        weapon(nullptr), 
        armor(nullptr), 
        helmet(nullptr),
        alive(true),
        health(100) // o el valor inicial que prefieras
    {}

    // Getters y Setters para cada atributo
    Position getPosition() const;
    void setPosition(const Position& newPosition);

    uint8_t getWeapon() const; // ahora en weapon
    void pickWeapon(Weapon* newWeapon);

    uint8_t getAmmo() const; // ahora en weapon
    void setAmmo(uint8_t newAmmo);
    void reload(uint8_t newAmmo);

    bool isAlive() const;
    void setAlive(bool isAlive);

    bool isFalling() const;
    void setFalling(bool isFalling);

    bool isCrouched() const;
    void setCrouched(bool isCrouched);

    bool hasArmorEquipped() const;
    void setArmorEquipped();

    bool hasHelmetEquipped() const;
    void setHelmetEquipped();

    uint8_t getHealth() const;
    void setHealth(uint8_t newHealth);

    uint8_t getFacingDirection() const;
    void setFacingDirection(uint8_t direction);
    void move(int dx, int dy);

    int getScore() const;
    void setScore(int newScore);

    uint8_t getDuckColor() const;
    void setDuckColor(uint8_t color);

    void takeDamage(uint8_t damage);

    void shoot();
    // Métodos adicionales para acciones del jugador
    void moveLeft();
    void moveRight();
    // Simulación de salto
    void jump() {
        if (!falling) {
            verticalVelocity = jumpStrength;
            falling = true;
        }
    }
    void crouch();

    void dropWeapon();
    void equipArmor();
    void equipHelmet();


    // Actualizar posición vertical considerando física
    void updatePosition(float deltaTime) {
        if (falling) {
            verticalVelocity += gravity * deltaTime;
            position.y += verticalVelocity * deltaTime;
            if (position.y <= 0) {  // Asumimos que y=0 es el suelo
                position.y = 0;
                verticalVelocity = 0;
                falling = false;
            }
        }
    }
};

#endif // PLAYERSTATE_H


// Notas:
/*
Hacer una clase Weapon y luego utilizar herencia.

*/