#ifndef CALCULATORMANAGER_H_
#define CALCULATORMANAGER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "zoom.h"
#include "duck.h"

class CalculatorManager {
public:
    static SDL_FPoint calculateCenter(const std::vector<std::unique_ptr<Duck>>& ducks) {
        float sumX = 0, sumY = 0;
        for (const auto& duck : ducks) {
            sumX += duck->getPosX();
            sumY += duck->getPosY();
        }
        return {sumX / ducks.size(), sumY / ducks.size()};
    }

    static float calculateMaxDistance(const std::vector<std::unique_ptr<Duck>>& ducks) {
        float maxDistance = 0;
        for (size_t i = 0; i < ducks.size(); ++i) {
            for (size_t j = i + 1; j < ducks.size(); ++j) {
                float dx = ducks[i]->getPosX() - ducks[j]->getPosX();
                float dy = ducks[i]->getPosY() - ducks[j]->getPosY();
                float distance = std::sqrt(dx * dx + dy * dy);
                maxDistance = std::max(maxDistance, distance);
            }
        }
        return maxDistance;
    }
};

#endif