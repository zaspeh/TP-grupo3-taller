#include "gameloop.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "server.h"
#include "../common_src/utils.h"

GameLoop::GameLoop(Server& server, Monitor& monitor, std::atomic<bool>& gameShouldContinue): continueGame(gameShouldContinue), server(server), cola_comandos(100), iteraciones(0), monitor(monitor) {
    gameState = std::make_unique<GameState>(continueGame);
}
                    
void GameLoop::initGame() {
    gameState = std::make_unique<GameState>(continueGame);
}

void GameLoop::addCommand(std::function<void()> command) {
    if (_keep_running)
        cola_comandos.push(std::move(command));
}

void GameLoop::ejecutar_comandos() {
    std::function<void()> command;
    while (_keep_running && cola_comandos.try_pop(command)) {
        command();
    }
}

void GameLoop::run() {
    constexpr float target_frame_duration = 1.0f / 30.0f; 
    auto last_time = std::chrono::steady_clock::now();
    auto next_game_tick = last_time;

    while (_keep_running) {
        auto current_time = std::chrono::steady_clock::now();
        std::chrono::duration<float> delta_time = current_time - last_time;
        last_time = current_time;
        ejecutar_comandos();
        
        if (gameState != nullptr) {
            monitor.procesar_mensaje(gameState->updatePlayers(delta_time.count()));
        }

        next_game_tick += std::chrono::duration_cast<std::chrono::steady_clock::duration>(
            std::chrono::duration<float>(target_frame_duration)
        );

        auto sleep_time = next_game_tick - std::chrono::steady_clock::now();
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            next_game_tick = std::chrono::steady_clock::now();
        }
    }
}

void GameLoop::doActionGameState(uint8_t player, uint8_t action) {
    if (_keep_running) {
        game_state_t gameStateStruct = gameState->doAction(player, action);
        monitor.procesar_mensaje(gameStateStruct);
    }
}

void GameLoop::stop() {
    if (!_keep_running) return;
    Thread::stop();
    std::function<void()> command;
    while (cola_comandos.try_pop(command)) {}
    cola_comandos.close();
}

GameLoop::~GameLoop() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr  << e.what() << std::endl;
    }
}