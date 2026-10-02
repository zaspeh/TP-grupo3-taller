#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class LTexture
{
public:
    LTexture(SDL_Renderer* renderer);
    ~LTexture();
    bool loadFromFile(std::string path);
    void free();
    void render(int x, int y, SDL_Rect* clip = NULL, SDL_Rect* scaleRect = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    SDL_Renderer* gRenderer;
    int mWidth;
    int mHeight;
};

#endif
