#include "PlayerState.h"

PlayerState::PlayerState()
    : position(0, 0), weapon(0), ammo(0), alive(true), falling(false) {}

// Getters
Position PlayerState::getPosition() const {
    return position;
}

uint8_t PlayerState::getWeapon() const { 
    return weapon->getId();
}

uint8_t PlayerState::getAmmo() const { 
    return weapon->getAmmo();
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

void PlayerState::setAmmo(uint8_t newAmmo) { 
    weapon->setAmmo(newAmmo);
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
    weapon->reload(newAmmo);
}

void PlayerState::takeDamage(uint8_t damage) {
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
        setAlive(false);
    }
}

void PlayerState::dropWeapon() { 
    weapon = null;
}

void PlayerState::pickWeapon(Weapon* newWeapon) {
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

uint8_t PlayerState::getHealth() {
    return health; 
}

void PlayerState::setHealth(uint8_t newHealth){
    health = newHealth;
}

uint8_t PlayerState::getFacingDirection() {
    return facingDirection;
}

void setFacingDirection(uint8_t direction){
    facingDirection = direction;
}

int PlayerState::getScore() {
    return score;
}

void PlayerState::setScore(uint8_t newScore){
    score = newScore;
}

uint8_t PlayerState::getDuckColor() {
    return duckColor;
}

void setDuckColor(uint8_t color) {
    duckColor = color;
}

bool PlayerState::hasArmorEquipped(){
    return armor->isEquipped();
}

void PlayerState::setArmorEquipped(){
    armor = Armor();
}

bool PlayerState::hasHelmetEquipped(){
    return helmet->isEquipped();
}

void PlayerState::setHelmetEquipped(){
    helmet = Helmet();
}


void PlayerState::shoot(){
    if (weapon != nullptr && !weapon->isEmpty()) {
        weapon->shoot();
    }
}

bool PlayerState::isCrouched() {
    return crouched;
}

void PlayerState::setCrouched(bool isCrouched){
    crouched = isCrouched;
}