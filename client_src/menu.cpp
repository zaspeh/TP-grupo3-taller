#include "menu.h"

Menu::Menu() : window(nullptr), renderer(nullptr), font(nullptr), 
               backgroundTexture(nullptr), logoTexture(nullptr),
               isRunning(false), gameStarted(false), isButtonHovered(false), showText(true) 
{
    buttonRect = {
        (WINDOW_WIDTH - BUTTON_WIDTH) / 2,
        BUTTON_TOP_MARGIN,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    };
}

Menu::~Menu() {
    clean();
}

bool Menu::init(SDL_Window* gWindow, SDL_Renderer* gRenderer) {
    window = gWindow;
    renderer = gRenderer;

    // Inicializar texturas
    backgroundTexture = new LTexture(renderer);
    logoTexture = new LTexture(renderer);

    // Cargar texturas
    if (!backgroundTexture->loadFromFile("client_src/forest.png")) {
        printf("Error cargando fondo\n");
        return false;
    }

    if (!logoTexture->loadFromFile("client_src/logo.png")) {
        printf("Error cargando logo\n");
        return false;
    }

    // Cargar fuente
    font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationMono-Bold.ttf", 20);
    if (!font) {
        printf("Error fuente: %s\n", TTF_GetError());
        return false;
    }

    blinkTimer = SDL_GetTicks();
    isRunning = true;
    return true;
}

void Menu::handleEvents() {
    SDL_Event event;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    isButtonHovered = mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
                     mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT && isButtonHovered && !gameStarted) {
                gameStarted = true;
                stop();
            }
        }
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            else if (event.key.keysym.sym == SDLK_RETURN && !gameStarted) {
                gameStarted = true;
                stop();
            }
        }
    }
}

void Menu::update() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - blinkTimer >= static_cast<Uint32>(BLINK_INTERVAL)) {
        showText = !showText;
        blinkTimer = currentTime;
    }
}

void Menu::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect backgroundRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    backgroundTexture->render(0, 0, nullptr, &backgroundRect);

    if (!gameStarted) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_Rect borderRect = buttonRect;
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_RenderDrawRect(renderer, &borderRect);

        borderRect.x += 2;
        borderRect.y += 2;
        borderRect.w -= 4;
        borderRect.h -= 4;
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderDrawRect(renderer, &borderRect);

        if (showText) {
            SDL_Color textColor = {255, 255, 255, 255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, "PRESS START", textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            SDL_Rect textRect = {
                buttonRect.x + (buttonRect.w - surface->w) / 2,
                buttonRect.y + (buttonRect.h - surface->h) / 2,
                surface->w,
                surface->h
            };

            SDL_RenderCopy(renderer, texture, NULL, &textRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_Rect logoRect = {
            (WINDOW_WIDTH - 600) / 2,
            WINDOW_HEIGHT / 2 - 100, 
            600,                      
            300                        
        };
        logoTexture->render(logoRect.x, logoRect.y, NULL, &logoRect);
    }
    else {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME STARTED!", textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect textRect = {
            (WINDOW_WIDTH - surface->w) / 2,
            (WINDOW_HEIGHT - surface->h) / 2,
            surface->w,
            surface->h
        };

        SDL_RenderCopy(renderer, texture, NULL, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

void Menu::clean() {
    if (backgroundTexture) {
        delete backgroundTexture;
        backgroundTexture = nullptr;
    }
    if (logoTexture) {
        delete logoTexture;
        logoTexture = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    IMG_Quit();
    TTF_Quit();
}


