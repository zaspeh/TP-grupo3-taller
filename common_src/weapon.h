// Weapon.h
#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon {
protected:
    uint8_t id;
    uint8_t ammo;
    int range;
    std::string name;
    bool isReloading;

public:
    Weapon(int initialAmmo, int weaponRange, const std::string& weaponName) : 
        ammo(initialAmmo), range(weaponRange), name(weaponName), isReloading(false) {}
    
    virtual ~Weapon() = default;
    
    virtual bool shoot() = 0;
    virtual bool canShoot() const { return ammo > 0 && !isReloading; }
    
    int getAmmo() const { return ammo; }
    void reload(uint8_t newAmmo) { ammo += newAmmo; }
    void setAmmo(uint8_t newAmmo) { ammo = newAmmo; }
    int getId() const { return id; }
    int getRange() const { return range; }
    const std::string& getName() const { return name; }
};

// Armas específicas
class Grenade : public Weapon {
private:
    bool pinPulled;
    float timeToExplode;
    static const int EXPLOSION_RADIUS = 5;

public:
    Grenade() : Weapon(1, 5, "Granada"), id(GRENADE_WEAPON) pinPulled(false), timeToExplode(4.0f) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        pinPulled = true;
        return true;
    }

    void throw_grenade() {
        if (pinPulled && ammo > 0) {
            ammo--;
            pinPulled = false;
        }
    }
};

class Banana : public Weapon {
public:
    Banana() : Weapon(1, 5, "Banana"), id(BANANA_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        ammo--;
        return true;
    }
};

class PewPewLaser : public Weapon {
private:
    static const int SHOTS_PER_BURST = 3;

public:
    PewPewLaser() : Weapon(12, 35, "Pew-Pew Laser"), id(PEWPEWLASER_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        ammo--;
        // Lógica para disparar 3 rayos con dispersión
        return true;
    }
};

class LaserRifle : public Weapon {
public:
    LaserRifle() : Weapon(10, 30, "Laser Rifle"), id(LASERRIFLE_WEAPON) {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        ammo--;
        return true;
    }
};



// banana, fewfew, laser podrian ser parte de lo mismo