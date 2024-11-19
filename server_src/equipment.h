#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <cstdint>

class Equipment {
protected:
    bool equipped; 

public:
    Equipment() : equipped(false) {}

    virtual void equip() { equipped = true; }
    virtual void unequip() { equipped = false; }
    bool isEquipped() const { return equipped; } 
    virtual ~Equipment() = default;
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

#endif // EQUIPMENT_H
