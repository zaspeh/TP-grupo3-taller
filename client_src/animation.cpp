#include "animation.h"

Animation::Animation(int qAnimationFrames, int spriteWidth, int spriteHeight, SDL_Renderer* renderer, DuckAnimationType type)
: frame(0),
  qAnimationFrames(qAnimationFrames), 
  spriteWidth(spriteWidth), 
  spriteHeight(spriteHeight), 
  type(type),
  gSpriteSheetTexture(renderer) {

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

    if (type == FLAPPING){
        for (int i = 0; i < qAnimationFrames; ++i){
            gSpriteClips[i].x = i * spriteWidth;
            gSpriteClips[i].y = spriteHeight;
            gSpriteClips[i].w = spriteWidth;
            gSpriteClips[i].h = spriteHeight;
        }
    }

    frame = 0;
}

bool Animation::loadTexture(std::string path){
    return gSpriteSheetTexture.loadFromFile(path);
}

void Animation::renderAnimation(float x, float y, SDL_Rect &scaleRect, bool faceLeft, bool motion) {
    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (motion && (currentTime - lastFrameTime) > 100) {
        frame = (frame + 1) % qAnimationFrames;
        lastFrameTime = currentTime;
    }
    
    SDL_Rect* currentClip = &gSpriteClips[frame]; 

    SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    gSpriteSheetTexture.render(x, y, currentClip, &scaleRect, flip);
}

