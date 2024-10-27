// Weapon.h
#ifndef WEAPON_H
#define WEAPON_H

#include <string>

class Weapon {
protected:
    int ammo;
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
    Grenade() : Weapon(1, 5, "Granada"), pinPulled(false), timeToExplode(4.0f) {}
    
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
    Banana() : Weapon(1, 5, "Banana") {}
    
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
    PewPewLaser() : Weapon(12, 35, "Pew-Pew Laser") {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        ammo--;
        // Lógica para disparar 3 rayos con dispersión
        return true;
    }
};

class LaserRifle : public Weapon {
public:
    LaserRifle() : Weapon(10, 30, "Laser Rifle") {}
    
    bool shoot() override {
        if (!canShoot()) return false;
        ammo--;
        return true;
    }
};

// Equipamiento
class Equipment {
protected:
    bool isEquipped;

public:
    Equipment() : isEquipped(false) {}
    virtual ~Equipment() = default;
    
    virtual void equip() { isEquipped = true; }
    virtual void unequip() { isEquipped = false; }
    bool isEquipped() const { return isEquipped; }
};

class Armor : public Equipment {
private:
    int protection;

public:
    Armor() : protection(1) {}
    
    int getProtection() const { return protection; }
    void absorb_hit() { protection--; if (protection <= 0) unequip(); }
};

class Helmet : public Equipment {
private:
    int protection;

public:
    Helmet() : protection(1) {}
    
    int getProtection() const { return protection; }
    void absorb_hit() { protection--; if (protection <= 0) unequip(); }
};

// banana, fewfew, laser podrian ser parte de lo mismo