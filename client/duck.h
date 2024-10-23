#ifndef DUCK_H
#define DUCK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ltexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_WIDTH = 32;
const int SPRITE_HEIGHT = 32;
const int WALKING_ANIMATION_FRAMES = 6;
const int JUMPING_ANIMATION_FRAMES = 6;
const float DUCK_SPEED = 1.0f;

class Duck
{
public:
    Duck(int screenWidth, int screenHeight, SDL_Renderer* renderer);
    void handleEvent(SDL_Event& e);
    void move();
    void render();
    bool loadTexture(std::string path);

private:
    float playerX, playerY;
    bool moveLeft, moveRight, faceLeft;
    bool isJumping;
    int frame;
    float jumpVelocity;
    float jumpHeight;
    float gravity;
    SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES + JUMPING_ANIMATION_FRAMES];
    LTexture gSpriteSheetTexture;
    SDL_Renderer* gRenderer;
    SDL_Rect scaleRect;
    int screenWidth;
    int screenHeight;
};


#endif
