#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <iostream>
#include <map>
#include "../common_src/utils.h"
#include "../common_src/game_state.h"
#include "../common_src/config_manager.h"

// Declaración de mapas globales.
extern std::map<int, float> weaponRecoil;
extern std::map<int, uint8_t> ammoForWeapons;

class Weapon {
protected:
    weapon_t weaponState;
    int range;

public:
    Weapon(weapon_t weaponSt, int weaponRange)
        : weaponState(weaponSt), range(weaponRange) {}

    virtual ~Weapon() = default;

    virtual bool shoot(bool infinitAmmo) = 0;

    virtual bool canShoot() const { return weaponState.ammo > 0; }

    int getAmmo() const { return weaponState.ammo; }

    void setAmmo(uint8_t newAmmo) { weaponState.ammo = newAmmo; }

    int getId() const { return weaponState.type; }

    int getRange() const { return range; }

    uint8_t getType() const { return weaponState.type; }

    float getRecoil() const { return weaponRecoil[weaponState.type]; }
};

// Clases de armas específicas.
class Grenade : public Weapon {
private:
    bool pinPulled;
    float timeToExplode;
    static const int EXPLOSION_RADIUS = 5;

public:
    Grenade(weapon_t weaponState) 
        : Weapon(weaponState, 5), pinPulled(false), timeToExplode(4.0f) {}

    bool shoot(bool infinitAmmo) override {
        if (!pinPulled) {
            pinPulled = true;
            return false;
        }
        return true;
    }

    bool getPinPulled() const { return pinPulled; }

    float getTimeToExplode() const { return timeToExplode; }
    void setTimeToExplode(float newTimeToExplode) { timeToExplode = newTimeToExplode; }

    void throw_grenade() {
        if (pinPulled) {
            if (getAmmo() > 0) {
                setAmmo(getAmmo() - 1);
            }
            pinPulled = false;
            timeToExplode = 4.0f;
        }
    }
};

class Banana : public Weapon {
public:
    Banana(weapon_t weaponState) : Weapon(weaponState, 5) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

// Resto de las clases de armas similares a Banana.
class Dartgun : public Weapon {
public:
    Dartgun(weapon_t weaponState) : Weapon(weaponState, 15) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class AK47 : public Weapon {
public:
    AK47(weapon_t weaponState) : Weapon(weaponState, 15) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class PewPewLaser : public Weapon {
public:
    PewPewLaser(weapon_t weaponState) : Weapon(weaponState, 35) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class LaserRifle : public Weapon {
public:
    LaserRifle(weapon_t weaponState) : Weapon(weaponState, 30) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class CowBoyPistol : public Weapon {
public:
    CowBoyPistol(weapon_t weaponState) : Weapon(weaponState, 30) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class Magnum : public Weapon {
public:
    Magnum(weapon_t weaponState) : Weapon(weaponState, 30) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class Shotgun : public Weapon {
public:
    Shotgun(weapon_t weaponState) : Weapon(weaponState, 30) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

class Sniper : public Weapon {
public:
    Sniper(weapon_t weaponState) : Weapon(weaponState, 30) {}

    bool shoot(bool infinitAmmo) override {
        if (!infinitAmmo) {
            if (!canShoot()) return false;
            setAmmo(getAmmo() - 1);
        }
        return true;
    }
};

#endif // WEAPON_H
