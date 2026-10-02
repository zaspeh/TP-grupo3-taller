#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "zoom.h"
#include <cmath>
#include <limits>
#include <iostream>

class Camera {
private:
    float x, y;
    float targetX, targetY;
    const float CAMERA_SMOOTHNESS;

public:
    Camera(float startX = SCREEN_WIDTH / 2.0f, float startY = SCREEN_HEIGHT / 2.0f, 
           float smoothness = 0.1f)
        : x(startX)
        , y(startY)
        , targetX(startX)
        , targetY(startY)
        , CAMERA_SMOOTHNESS(smoothness) {}

    void update(float newTargetX, float newTargetY, const Zoom& zoom) {
        float visibleWorldWidth = zoom.getVisibleWidth();
        float visibleWorldHeight = zoom.getVisibleHeight();

        float maxX = WORLD_WIDTH - (visibleWorldWidth / 2);
        float maxY = WORLD_HEIGHT - (visibleWorldHeight / 2);
        float minX = visibleWorldWidth / 2;
        float minY = visibleWorldHeight / 2;
        
        targetX = std::max(minX, std::min(maxX, newTargetX));
        targetY = std::max(minY, std::min(maxY, newTargetY));
        
        x += (targetX - x) * CAMERA_SMOOTHNESS;
        y += (targetY - y) * CAMERA_SMOOTHNESS;
        
        x = std::max(minX, std::min(maxX, x));
        y = std::max(minY, std::min(maxY, y));
    }

    SDL_Point getScreenPosition(float worldX, float worldY, float zoom) const {
        return {
            static_cast<int>((worldX - x) * zoom + SCREEN_WIDTH / 2),
            static_cast<int>((worldY - y) * zoom + SCREEN_HEIGHT / 2)
        };
    }

    SDL_Rect getBackgroundRect(int textureWidth, int textureHeight, float zoom) const {
        float scaleX = static_cast<float>(WORLD_WIDTH) / textureWidth;
        float scaleY = static_cast<float>(WORLD_HEIGHT) / textureHeight;

        return {
            static_cast<int>((-x) * zoom + SCREEN_WIDTH / 2),
            static_cast<int>((-y) * zoom + SCREEN_HEIGHT / 2),
            static_cast<int>(textureWidth * scaleX * zoom),
            static_cast<int>(textureHeight * scaleY * zoom)
        };
    }

    float getX() const { return x; }
    float getY() const { return y; }
};

#endif
