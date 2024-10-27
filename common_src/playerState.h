#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "Position.h"

class PlayerState {
private:
    Position position;    // Posición del jugador en el mapa
    uint8_t weapon;       // Identificador del arma actual
    uint8_t ammo;         // Cantidad de municiones disponibles para el arma actual
    bool alive;           // Estado de vida del jugador
    bool falling;         // Si el jugador está en caída libre
    bool flapping;        // Si el jugador está aleteando para caer más despacio
    bool crouched;        // Si el jugador está tirado en el piso simulando estar muerto
    bool hasArmor;        // Si el jugador tiene una armadura equipada
    bool hasHelmet;       // Si el jugador tiene un casco equipado
    uint8_t health;       // Cantidad de "vidas" o "golpes" que el jugador puede recibir
    int facingDirection;  // Dirección en la que el jugador está mirando (-1 izquierda, 1 derecha)
    int score;            // Puntuación del jugador
    uint8_t duckColor;    // Color asignado al pato para identificar al jugador
    Weapon* weapon;
    Armor* armor;
    Helmet* helmet;

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

    uint8_t getWeapon() const;
    void setWeapon(uint8_t newWeapon);

    uint8_t getAmmo() const;
    void setAmmo(uint8_t newAmmo);

    bool isAlive() const;
    void setAlive(bool isAlive);

    bool isFalling() const;
    void setFalling(bool isFalling);

    bool isFlapping() const;
    void setFlapping(bool isFlapping);

    bool isCrouched() const;
    void setCrouched(bool isCrouched);

    bool hasArmorEquipped() const;
    void setArmorEquipped(bool hasArmor);

    bool hasHelmetEquipped() const;
    void setHelmetEquipped(bool hasHelmet);

    uint8_t getHealth() const;
    void setHealth(uint8_t newHealth);

    int getFacingDirection() const;
    void setFacingDirection(int direction);

    int getScore() const;
    void setScore(int newScore);

    uint8_t getDuckColor() const;
    void setDuckColor(uint8_t color);

    // Métodos adicionales para acciones del jugador
    void moveLeft();
    void moveRight();
    void jump();
    void crouch();
    void shoot();
    void pickUpWeapon(uint8_t newWeapon);
    void dropWeapon();
    void equipArmor();
    void equipHelmet();
    void takeDamage(uint8_t damage);
    void die();
    void pickWeapon(Weapon* newWeapon);
};

#endif // PLAYERSTATE_H


// Notas:
/*
Hacer una clase Weapon y luego utilizar herencia.

*/