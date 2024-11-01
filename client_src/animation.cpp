#include "animation.h"

Animation::Animation(int qAnimationFrames, int spriteWidth, int spriteHeight, SDL_Renderer* renderer, DuckAnimationType type)
: gSpriteSheetTexture(renderer), qAnimationFrames(qAnimationFrames), spriteWidth(spriteWidth), spriteHeight(spriteHeight), type(type){

    if (type == WALKING){
        for (int i = 0; i < qAnimationFrames; ++i){
            gSpriteClips[i].x = i * spriteWidth;
            gSpriteClips[i].y = 0;
            gSpriteClips[i].w = spriteWidth;
            gSpriteClips[i].h = spriteHeight;
        }
    }

    if (type == JUMPING){
        for (int i = 0; i < qAnimationFrames; ++i){
            gSpriteClips[i].x = i * spriteWidth;
            gSpriteClips[i].y = spriteHeight;
            gSpriteClips[i].w = spriteWidth;
            gSpriteClips[i].h = spriteHeight;
        }
    }

    if (type == DUCKING){
        for (int i = 0; i < qAnimationFrames; ++i){
            gSpriteClips[i].x = i * spriteWidth;
            gSpriteClips[i].y = spriteHeight*2;
            gSpriteClips[i].w = spriteWidth;
            gSpriteClips[i].h = spriteHeight;
        }
    }

    frame = 0;
}

bool Animation::loadTexture(std::string path){
    return gSpriteSheetTexture.loadFromFile(path);
}

void Animation::renderAnimation(float x, float y, SDL_Rect &scaleRect, bool faceLeft){
    SDL_Rect* currentClip;
    currentClip = &gSpriteClips[frame / qAnimationFrames];

    SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    gSpriteSheetTexture.render(x, y, currentClip, &scaleRect, flip);
    frame++;
    if (frame / qAnimationFrames >= qAnimationFrames){
        frame = 0;
    }
}
