#include "ltexture.h"

LTexture::LTexture(SDL_Renderer* renderer)
    : mTexture(NULL), gRenderer(renderer), mWidth(0), mHeight(0) {}

LTexture::~LTexture()
{
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    if (path.empty()) {
        printf("Empty path provided to loadFromFile\n");
        return false;
    }
    if (gRenderer == NULL) {
        printf("Renderer is NULL in loadFromFile\n");
        return false;
    }
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (mTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
    SDL_FreeSurface(loadedSurface);
    return true;
}

void LTexture::free() {
    if (mTexture != nullptr && gRenderer != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip, SDL_Rect* scaleRect, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (scaleRect != NULL)
    {
        renderQuad.w = scaleRect->w;
        renderQuad.h = scaleRect->h;
    }
    else if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, 0, NULL, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}
