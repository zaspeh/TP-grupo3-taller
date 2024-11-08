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

    // Carga la textura desde un archivo de imagen
    bool loadFromFile(std::string path);

    // Libera los recursos de la textura
    void free();

    // Renderiza la textura en las coordenadas especificadas
    void render(int x, int y, SDL_Rect* clip = NULL, SDL_Rect* scaleRect = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    // Devuelve el ancho y alto de la textura
    int getWidth();
    int getHeight();

private:
    SDL_Texture* mTexture;
    SDL_Renderer* gRenderer;
    int mWidth;
    int mHeight;
};

#endif
