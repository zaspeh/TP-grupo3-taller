#include "player_state.h"

// Other methods
void PlayerState::move(int dx, int dy) {
    duck.pos.x += dx;
    duck.pos.y += dy;
    //std::cout << "Posicion del pato: " << duck.pos.x << " " << duck.pos.y << std::endl;
}

void PlayerState::reload(uint8_t newAmmo) { 
    weapon->reload(newAmmo);
}

void PlayerState::takeDamage(uint8_t damage) {
    if (armor.isEquipped()) {
        armor.absorb_hit();
        return;
    }
    if (helmet.isEquipped()) {
        helmet.absorb_hit();
        return;
    }
    
    duck.health -= damage;
    if (duck.health <= 0) {
        setAlive();
    }
}

void PlayerState::pickWeapon(Weapon* newWeapon) {
    if (weapon) {
        dropWeapon();
    }
    weapon = newWeapon;
    duck.equipped_weapon.is_equipped = true;
    duck.equipped_weapon.type = newWeapon->getId();
    duck.equipped_weapon.ammo = newWeapon->getAmmo();
}

void PlayerState::dropWeapon() {
    if (weapon != nullptr) {
        duck.equipped_weapon.is_equipped = false;
        weapon = nullptr;
    }
}

void PlayerState::shoot(){
    if (weapon != nullptr && !weapon->canShoot()) {
        weapon->shoot(); // se hacen los cambios de balas y eso
        duck.equipped_weapon.ammo = weapon->getAmmo();
    }
}
