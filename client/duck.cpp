#include "duck.h"

Duck::Duck(int screenWidth, int screenHeight, SDL_Renderer* renderer)
    : playerX((screenWidth - SPRITE_WIDTH) / 2), playerY((screenHeight - SPRITE_HEIGHT) / 2),
      moveLeft(false), moveRight(false), faceLeft(false), frame(0),
      isJumping(false), jumpVelocity(0.0f), gravity(0.2f), jumpHeight(10.0f), 
      gSpriteSheetTexture(renderer), gRenderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight)
{
    scaleRect.w = SPRITE_WIDTH * 2;
    scaleRect.h = SPRITE_HEIGHT * 2;

    for (int i = 0; i < WALKING_ANIMATION_FRAMES; ++i)
    {
        gSpriteClips[i].x = i * SPRITE_WIDTH;
        gSpriteClips[i].y = 0;
        gSpriteClips[i].w = SPRITE_WIDTH;
        gSpriteClips[i].h = SPRITE_HEIGHT;
    }

    for (int i = WALKING_ANIMATION_FRAMES; i < WALKING_ANIMATION_FRAMES+JUMPING_ANIMATION_FRAMES; ++i)
    {
        gSpriteClips[i].x = i * SPRITE_WIDTH;
        gSpriteClips[i].y = SPRITE_HEIGHT;
        gSpriteClips[i].w = SPRITE_WIDTH;
        gSpriteClips[i].h = SPRITE_HEIGHT;
    }
}

bool Duck::loadTexture(std::string path)
{
    return gSpriteSheetTexture.loadFromFile(path);
}

void Duck::handleEvent(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            moveLeft = true;
            faceLeft = true;
            break;
        case SDLK_RIGHT:
            moveRight = true;
            faceLeft = false;
            break;
        case SDLK_UP: 
            if (!isJumping)
            {
                isJumping = true;
                jumpVelocity = -10.5f;
            }
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            moveLeft = false;
            break;
        case SDLK_RIGHT:
            moveRight = false;
            break;
        }
    }
}



void Duck::move()
{
    if (isJumping)
    {
        playerY += jumpVelocity;
        jumpVelocity += gravity;

        if (playerY >= screenHeight - SPRITE_HEIGHT * 2)
        {
            playerY = screenHeight - SPRITE_HEIGHT * 2;
            isJumping = false;
            jumpVelocity = 0.0f; 
        }
    }
    else
    {
        if (moveLeft || moveRight)
        {
            ++frame;
            if (frame / 6 >= WALKING_ANIMATION_FRAMES)
            {
                frame = 0;
            }

            if (moveLeft)
            {
                playerX -= DUCK_SPEED;
            }
            else if (moveRight)
            {
                playerX += DUCK_SPEED;
            }

            if (playerX < 0)
            {
                playerX = 0;
            }
            if (playerX > screenWidth - SPRITE_WIDTH * 2)
            {
                playerX = screenWidth - SPRITE_WIDTH * 2;
            }
        }
    }

    if (playerY < (screenHeight - SPRITE_HEIGHT * 2 - jumpHeight) && isJumping)
    {
        jumpVelocity = gravity;
    }
}

void Duck::render()
{
    SDL_Rect* currentClip;

    if (isJumping)
    {
        currentClip = &gSpriteClips[WALKING_ANIMATION_FRAMES + (frame / 12)];
    }
    else
    {
        currentClip = &gSpriteClips[frame / 6];
    }

    SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    gSpriteSheetTexture.render(playerX, playerY, currentClip, &scaleRect, flip);
}
