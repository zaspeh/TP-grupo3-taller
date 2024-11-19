#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ltexture.h"

#define MAX_ANIMATIONS 10

enum DuckAnimationType { WALKING, JUMPING, DUCKING, FLAPPING, FIRE };

class Animation {
public:
    Animation(int qAnimationFrames, int spriteWidth, int spriteHeight, SDL_Renderer* renderer, DuckAnimationType type);
    bool loadTexture(std::string path);
    void renderAnimation(float x, float y, SDL_Rect &scaleRect, bool faceLeft, bool motion);
private:
    int frame;
    int qAnimationFrames;
    int spriteWidth;
    int spriteHeight;
    DuckAnimationType type;
    SDL_Rect gSpriteClips[MAX_ANIMATIONS];
    LTexture gSpriteSheetTexture;
};

#endif
