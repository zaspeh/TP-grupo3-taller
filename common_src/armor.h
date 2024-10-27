#ifndef ARMOR_H
#define ARMOR_H

#include <cstdint>

class Armor {
private:
    uint8_t durability;

public:
    explicit Armor(uint8_t initialDurability) : durability(initialDurability) {}

    bool absorbDamage() {
        if (durability > 0) {
            --durability;
            return true; // Absorbió el impacto
        }
        return false; // La armadura ya no puede absorber más
    }

    uint8_t getDurability() const {
        return durability;
    }
};

#endif // ARMOR_H
