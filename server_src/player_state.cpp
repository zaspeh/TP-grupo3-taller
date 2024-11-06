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



/*
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
*/

/*
typedef struct {
    position_t pos;
    uint8_t type;    // Tipo de arma (ej.: pistola, escopeta, etc.)
    int ammo;        // Munición restante del arma
    bool is_equipped; // Indica si el arma está equipada por un jugador
} weapon_t;
*/
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
