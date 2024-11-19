#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <atomic>
#include <functional>
#include <utility>

#include "../common_src/queue.h"
#include "../common_src/thread.h"
#include "game_state.h"
#include "monitor.h"

class Server;
class GameState;

class GameLoop: public Thread {
private:
    Server& server;
    Queue<std::function<void()>> cola_comandos;
    std::atomic<int> iteraciones;
    std::unique_ptr<GameState> gameState;
    Monitor& monitor;

    void ejecutar_comandos();

public:
    bool matchStarted = false;
    explicit GameLoop(Server& server, Monitor& monitor);
    void initGame();
    void doActionGameState(uint8_t player, uint8_t action);
    void run() override;
    void stop() override;
    void agregar_comando(std::function<void()> command);
    void removePlayer(uint8_t idPlayer);
    ~GameLoop();

    float getCurrentTime(){
        return std::chrono::duration<float>(
            std::chrono::steady_clock::now().time_since_epoch()
        ).count();
     }
};

#endif  // GAMELOOP_H