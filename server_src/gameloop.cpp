#include "gameloop.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "server.h"
#include "../common_src/utils.h"

GameLoop::GameLoop(Server& server, Monitor& monitor): server(server), cola_comandos(100), iteraciones(0), gameState(nullptr), monitor(monitor) {}
    
bool GameLoop::firstTime(uint8_t id) {
    return gameState->isPlayerConnected(id);
}
                    
void GameLoop::initGame() {
    gameState = std::unique_ptr<GameState>(new GameState());
}

void GameLoop::agregar_comando(std::function<void()> command) {
    cola_comandos.push(std::move(command));
}

void GameLoop::ejecutar_comandos() {
    std::function<void()> command;
    while (_keep_running && cola_comandos.try_pop(command)) {
        command();
    }
}
/*
void GameLoop::run() {
    constexpr float target_frame_duration = 1.0f / 30.0f; // Duración del frame objetivo (30 FPS)
    auto next_frame = std::chrono::steady_clock::now();   // Tiempo del próximo frame

    while (_keep_running) {
        auto start = std::chrono::steady_clock::now();

        // Ejecutar comandos en cola
        ejecutar_comandos();

        // Calcular deltaTime para actualizar el estado de los jugadores
        auto deltaTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
        if(gameState != nullptr)
            gameState->updatePlayers(deltaTime);

        // Calcular el tiempo de finalización y duración de la iteración
        next_frame += std::chrono::duration_cast<std::chrono::steady_clock::duration>(
            std::chrono::duration<float>(target_frame_duration)
        );  // Conversión a la duración del reloj

        auto end = std::chrono::steady_clock::now();

        // Tiempo restante para completar el frame a 30 FPS
        auto remaining_time = std::chrono::duration_cast<std::chrono::milliseconds>(next_frame - end);
        if (remaining_time.count() > 0) {
            std::this_thread::sleep_for(remaining_time);
        }
    }
}
*/
    void GameLoop::run() {
        float currentTime = getCurrentTime();
        float lastTime = currentTime;
        float nextGameTick = currentTime;
        
        while (_keep_running) {
            currentTime = getCurrentTime();
            float deltaTime = currentTime - lastTime;  
            lastTime = currentTime;
            
            ejecutar_comandos();

            // Actualizar estado del juego
            if (gameState != nullptr) {
                monitor.procesar_mensaje(gameState->updatePlayers(deltaTime));
            }
            
            // Calcular el próximo tick
            nextGameTick += 1.0f / 30.0f;
            
            // Dormir hasta el próximo frame si es necesario
            float sleepTime = nextGameTick - getCurrentTime();
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::duration<float>(sleepTime));
            } else {
                // Si nos estamos quedando atrás, reajustar
                nextGameTick = getCurrentTime();
            }
        }
    }


void GameLoop::doActionGameState(uint8_t player, uint8_t action) {
    game_state_t gameStateStruct = gameState->doAction(player, action);
    //std::cout << "Posición del pato - gameloop: " << gameStateStruct.level.ducks[0].pos.x << " " << gameStateStruct.level.ducks[0].pos.y << std::endl;
    monitor.procesar_mensaje(gameStateStruct);
}

void GameLoop::stop() {
    Thread::stop();
    cola_comandos.close();
}

GameLoop::~GameLoop() {
    if (!_keep_running)
        return;

    try {
        Thread::stop();
        cola_comandos.close();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}