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

class GameLoop: public Thread {
private:
    Server& server;
    Queue<std::function<void()>> cola_comandos;
    std::atomic<int> iteraciones;
    std::unique_ptr<GameState> gameState;
    Monitor& monitor;

    // Ejecuta los comandos pendientes en la cola de comandos.
    void ejecutar_comandos();

    // Simula una iteración del bucle de juego.
    void simular_iteracion();

public:
    bool matchStarted = false;
    // Constructor que inicializa el bucle de juego con una referencia al servidor.
    explicit GameLoop(Server& server, Monitor& monitor);
    
    void initGame();

    void doActionGameState(uint8_t player, uint8_t action);

    bool firstTime(uint8_t player);
    
    // Método que contiene la lógica del bucle de juego y se ejecuta en un hilo separado.
    void run() override;

    // Detiene el bucle de juego.
    void stop() override;

    // Agrega un comando a la cola de comandos para su ejecución posterior.
    void agregar_comando(std::function<void()> command);

    // Retorna el número de iteraciones que ha realizado el bucle de juego.
    int obtener_iteraciones() const { return iteraciones; }

    // Destruye el gameloop, liberando todos los recursos reservados.
    ~GameLoop();
};

#endif  // GAMELOOP_H