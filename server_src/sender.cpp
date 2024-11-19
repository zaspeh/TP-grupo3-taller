#include "sender.h"

#include <string>
#include <utility>

#include "monitor.h"
#include "server.h"

Sender::Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
               std::shared_ptr<Queue<game_state_t>> gameStateQueue):
        server(server), monitor(monitor), protocol(protocol), gameStateQueue(gameStateQueue) {}

void Sender::run() {
    while (_keep_running && server.esta_corriendo()) {
        game_state_t mensaje;
        try {
            if (isQueueClosed() || protocol == nullptr) 
                break;
            
            try {
                mensaje = gameStateQueue->pop();
            } catch (const std::exception& e) {
                std::cerr << "Error: fallo al recibir mensaje - " << e.what() << std::endl;
                break;
            }


            if (protocol == nullptr || !server.esta_corriendo()) 
                break;

            bool wasClosed = false;
            protocol->sendGameState(mensaje, wasClosed);

            if (wasClosed) 
                break;

        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << "sender - " << e.what() << std::endl;
            break;
        }
    }
}

void Sender::broadcast_message_with_info(game_state_t gameState) {
    if (gameStateQueue->isClosed()) {
        return;  
    }
    gameStateQueue->push(gameState);
}

void Sender::stop() {
    Thread::stop();
    game_state_t msg;
    while (gameStateQueue->try_pop(msg)) {}
    gameStateQueue->close();
}

Sender::~Sender() {
    if (!_keep_running)
        return;

    try {
        Thread::stop();
        game_state_t msg;
        while (gameStateQueue->try_pop(msg)) {}
        gameStateQueue->close();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}