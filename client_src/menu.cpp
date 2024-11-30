#include "menu.h"

Menu::Menu() : window(nullptr), renderer(nullptr), font(nullptr), 
               backgroundTexture(nullptr), logoTexture(nullptr),
               isRunning(false), gameStarted(false), isButtonPlayHovered(false), isButtonPlayPressed(false), showText(true), closed(false), chosenColor(0)
{
    buttonPlayRect = {
        (WINDOW_WIDTH - BUTTON_WIDTH) / 2,
        BUTTON_TOP_MARGIN,
        BUTTON_WIDTH,
        BUTTON_HEIGHT
    };

    int window_width = 1024;
    int window_height = 720;
    int button_width = 160;
    int button_height = 230;

    // Calcular márgenes horizontales y verticales
    int espacio_horizontal_total = window_width - (button_width * 4);
    int margen_horizontal = espacio_horizontal_total / 5;

    int espacio_vertical_total = window_height - button_height;
    int margen_vertical = espacio_vertical_total / 2;

    // Definir las posiciones de los botones
    buttonYellowDuckRect = { margen_horizontal, margen_vertical, button_width, button_height };
    buttonGrayDuckRect = { margen_horizontal * 2 + button_width, margen_vertical, button_width, button_height };
    buttonOrangeDuckRect = { margen_horizontal * 3 + button_width * 2, margen_vertical, button_width, button_height };
    buttonWhiteDuckRect = { margen_horizontal * 4 + button_width * 3, margen_vertical, button_width, button_height };

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
    yellowDuck = new LTexture(renderer);
    orangeDuck = new LTexture(renderer);
    grayDuck = new LTexture(renderer);
    whiteDuck = new LTexture(renderer);

    // Cargar texturas
    if (!backgroundTexture->loadFromFile("client_src/forest.png")) {
        printf("Error cargando fondo\n");
        return false;
    }

    if (!logoTexture->loadFromFile("client_src/logo.png")) {
        printf("Error cargando logo\n");
        return false;
    }

    if (!yellowDuck->loadFromFile("client_src/yellowplayer.png")) {
        printf("Error cargando player\n");
        return false;
    }

    if (!orangeDuck->loadFromFile("client_src/orangeplayer.png")) {
        printf("Error cargando player\n");
        return false;
    }

    if (!whiteDuck->loadFromFile("client_src/whiteplayer.png")) {
        printf("Error cargando player\n");
        return false;
    }

    if (!grayDuck->loadFromFile("client_src/grayplayer.png")) {
        printf("Error cargando player\n");
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

bool Menu::isButtonHovered(SDL_Rect button, int mouseX, int mouseY) {
    return mouseX >= button.x && mouseX <= button.x + button.w &&
           mouseY >= button.y && mouseY <= button.y + button.h;
}

void Menu::handleEvents() {
    SDL_Event event;
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    isButtonPlayHovered = isButtonHovered(buttonPlayRect,mouseX, mouseY);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
            closed = true;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT && isButtonPlayHovered && !gameStarted) {
                isButtonPlayPressed = true;
            }
            if (event.button.button == SDL_BUTTON_LEFT && isButtonPlayPressed && !gameStarted){
                if(isButtonHovered(buttonYellowDuckRect, mouseX, mouseY)){
                    chosenColor = 0;
                    gameStarted = true;
                    stop();
                }else if(isButtonHovered(buttonGrayDuckRect, mouseX, mouseY)){
                    chosenColor = 1;
                    gameStarted = true;
                    stop();
                }else if(isButtonHovered(buttonOrangeDuckRect, mouseX, mouseY)){
                    chosenColor = 2;
                    gameStarted = true;
                    stop();
                }else if(isButtonHovered(buttonWhiteDuckRect, mouseX, mouseY)){
                    chosenColor = 3;
                    gameStarted = true;
                    stop();
                }
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
        if (!isButtonPlayPressed){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &buttonPlayRect);

            SDL_Rect borderRect = buttonPlayRect;
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
                    buttonPlayRect.x + (buttonPlayRect.w - surface->w) / 2,
                    buttonPlayRect.y + (buttonPlayRect.h - surface->h) / 2,
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
        }else{
            yellowDuck->render(buttonYellowDuckRect.x, buttonYellowDuckRect.y, NULL, &buttonYellowDuckRect, SDL_FLIP_NONE);
            grayDuck->render(buttonGrayDuckRect.x, buttonGrayDuckRect.y, NULL, &buttonGrayDuckRect, SDL_FLIP_NONE);
            orangeDuck->render(buttonOrangeDuckRect.x, buttonOrangeDuckRect.y, NULL, &buttonOrangeDuckRect, SDL_FLIP_NONE);
            whiteDuck->render(buttonWhiteDuckRect.x, buttonWhiteDuckRect.y, NULL, &buttonWhiteDuckRect, SDL_FLIP_NONE);
        }
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
    if (yellowDuck) {
        delete yellowDuck;
        yellowDuck = nullptr;
    }
    if (orangeDuck) {
        delete orangeDuck;
        orangeDuck = nullptr;
    }
    if (grayDuck) {
        delete grayDuck;
        grayDuck = nullptr;
    }
    if (whiteDuck) {
        delete whiteDuck;
        whiteDuck = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    IMG_Quit();
    TTF_Quit();
}


