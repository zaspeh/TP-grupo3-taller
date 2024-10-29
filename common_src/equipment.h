#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <cstdint>

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

#endif // EQUIPMENT_H
