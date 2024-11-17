// Weapon.h
#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <iostream>
#include "../common_src/game_state.h"
#include "../common_src/utils.h"


class Weapon {
protected:
    weapon_t weapon;
    int id;
    int range;
    int ammo;

public:
    Weapon(int initialAmmo, int weaponRange, int id) :
        id(id),
        range(weaponRange) {
            ammo = initialAmmo;
        }
    
    virtual ~Weapon() = default;
    
    virtual bool shoot() = 0;
    virtual bool canShoot() const { return ammo > 0; }
    
    int getAmmo() const { return ammo; }
    void setAmmo(uint8_t newAmmo) { 
        std::cout << "Municiones: " << static_cast<int>(newAmmo) << std::endl;
        ammo = newAmmo; }
    int getId() const { return id; }
    int getRange() const { return range; }
    
    uint8_t getType() const { return id; }
};

// Armas específicas
class Grenade : public Weapon {
private:
    bool pinPulled;
    float timeToExplode;
    static const int EXPLOSION_RADIUS = 5;

public:
    Grenade() : Weapon(1, 5, GRENADE_WEAPON), pinPulled(false), timeToExplode(4.0f) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        pinPulled = true;
        return true;
    }

    void throw_grenade() {
        if (pinPulled && getAmmo() > 0) {
            setAmmo(getAmmo() - 1);
            pinPulled = false;
        }
    }
};

class Banana : public Weapon {
public:
    Banana() : Weapon(1, 5, BANANA_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class Dartgun : public Weapon {
public:
    Dartgun() : Weapon(10, 15, DARTGUN_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class AK47 : public Weapon {
public:
    AK47() : Weapon(20, 15, AK_47_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class PewPewLaser : public Weapon {
private:
    static const int SHOTS_PER_BURST = 3;

public:
    PewPewLaser() : Weapon(12, 35, PEWPEWLASER_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        // Lógica para disparar 3 rayos con dispersión
        return true;
    }
};

class LaserRifle : public Weapon {
public:
    LaserRifle() : Weapon(10, 30, LASERRIFLE_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
}; 

class CowBoyPistol : public Weapon {
public:
    CowBoyPistol() : Weapon(10, 30, COWBOY_WEAPON) {}

    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class Magnum : public Weapon {
public:
    Magnum() : Weapon(10, 30, MAGNUM_WEAPON) {}

    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class Shotgun : public Weapon {
public:
    Shotgun() : Weapon(10, 30, SHOTGUN_WEAPON) {}

    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

class Sniper : public Weapon {
public:
    Sniper() : Weapon(10, 30, SNIPER_WEAPON) {}

    bool shoot() override {
        if (!canShoot()) return false;
        setAmmo(getAmmo() - 1);
        return true;
    }
};

#endif // WEAPON_H
