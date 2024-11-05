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


                    /*
                    game_state_t state;
                    std::map<uint8_t, std::shared_ptr<PlayerState>> players;  
                    std::vector<std::shared_ptr<level_t>> levels;
                    Level level;
                    mutable std::mutex mtx;
                    */
                    
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

/* void GameLoop::simular_iteracion() {
    try {

        auto& cajas = server.obtener_cajas();

        for (auto& par: cajas) {
            auto& caja = par.second;
            if (!_keep_running) {
                break;
            }
            if (caja.esta_disponible(iteraciones) && caja.fue_caja_recogida()) {
                caja.actualizar_estado();
                server.obtener_monitor().notificar_reaparicion_caja();
            }
        }

        iteraciones++;
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
} */

void GameLoop::run() {
    while (_keep_running) {
        auto start = std::chrono::steady_clock::now();
        ejecutar_comandos();
        //simular_iteracion();

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (duration.count() < 1/30) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1/30) - duration);
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