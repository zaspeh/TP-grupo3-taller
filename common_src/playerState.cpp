#include "PlayerState.h"

PlayerState::PlayerState()
    : position(0, 0), weapon(0), ammo(0), alive(true), falling(false) {}

// Getters
Position PlayerState::getPosition() const {
    return position;
}

uint8_t PlayerState::getWeapon() const {
    return weapon;
}

uint8_t PlayerState::getAmmo() const {
    return ammo;
}

bool PlayerState::isAlive() const {
    return alive;
}

bool PlayerState::isFalling() const {
    return falling;
}

// Setters
void PlayerState::setPosition(const Position& newPosition) {
    position = newPosition;
}

void PlayerState::setWeapon(uint8_t newWeapon) {
    weapon = newWeapon;
}

void PlayerState::setAmmo(uint8_t newAmmo) {
    ammo = newAmmo;
}

void PlayerState::setAlive(bool newAliveStatus) {
    alive = newAliveStatus;
}

void PlayerState::setFalling(bool newFallingStatus) {
    falling = newFallingStatus;
}

// Other methods
void PlayerState::move(int dx, int dy) {
    position.x += dx;
    position.y += dy;
}

void PlayerState::reload(uint8_t newAmmo) {
    ammo = newAmmo;
}

void takeDamage(uint8_t damage) {
    if (armor && armor->isEquipped()) {
        armor->absorb_hit();
        return;
    }
    if (helmet && helmet->isEquipped()) {
        helmet->absorb_hit();
        return;
    }
    
    health -= damage;
    if (health <= 0) {
        die();
    }
}

void PlayerState::pickWeapon(uint8_t newWeapon, uint8_t weaponAmmo) {
    weapon = newWeapon;
    ammo = weaponAmmo;
}

void PlayerState::dropWeapon() {
    weapon = 0;
    ammo = 0;
}

void pickWeapon(Weapon* newWeapon) {
    if (weapon) {
        dropWeapon();
    }
    weapon = newWeapon;
}

void PlayerState::dropWeapon() {
    if (weapon != nullptr) {
        weapon->drop();
        weapon = nullptr;
    }
}

bool PlayerState::shootWeapon() {
    if (weapon != nullptr && !weapon->isEmpty()) {
        return weapon->shoot();
    }
    return false;
}
