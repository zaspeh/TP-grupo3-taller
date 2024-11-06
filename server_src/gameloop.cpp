#include "gameloop.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "server.h"
#include "../common_src/utils.h"

GameLoop::GameLoop(Server& server, Monitor& monitor): server(server), cola_comandos(100), iteraciones(0), gameState(nullptr), monitor(monitor) {}
    
game_state_t* GameLoop::firstTime(uint8_t id) {
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

void GameLoop::run() {
    constexpr float target_frame_duration = 1.0f / 30.0f; // Duración del frame objetivo (30 FPS)
    auto last_time = std::chrono::steady_clock::now();
    auto next_game_tick = last_time;

    while (_keep_running) {
        // Calcular el tiempo actual y deltaTime
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta_time = current_time - last_time;
        last_time = current_time;
        
        ejecutar_comandos();

        // Actualizar estado del juego
        if (gameState != nullptr) {
            monitor.procesar_mensaje(gameState->updatePlayers(delta_time.count()));
        }

        // Calcular el próximo tick
        next_game_tick += std::chrono::duration_cast<std::chrono::steady_clock::duration>(
            std::chrono::duration<float>(target_frame_duration)
        );

        // Dormir hasta el próximo frame si es necesario
        auto sleep_time = next_game_tick - std::chrono::steady_clock::now();
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            // Si nos estamos quedando atrás, reajustar
            next_game_tick = std::chrono::steady_clock::now();
        }
    }
}

void GameLoop::doActionGameState(uint8_t player, uint8_t action) {
    game_state_t gameStateStruct = gameState->doAction(player, action);
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