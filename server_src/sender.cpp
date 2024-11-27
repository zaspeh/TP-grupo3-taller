#include "sender.h"

#include <string>
#include <utility>

#include "monitor.h"
#include "server.h"

Sender::Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
               std::shared_ptr<Queue<game_state_t>> gameStateQueue):
        server(server), monitor(monitor), protocol(protocol), gameStateQueue(gameStateQueue) {}

void Sender::run() {
    while (_keep_running) {
        game_state_t mensaje;
        try {
            if (isQueueClosed() || protocol == nullptr) 
                break;
            
            try {
                mensaje = gameStateQueue->pop();
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                break;
            }

            if (protocol == nullptr) 
                break;

            bool wasClosed = false;
            protocol->sendGameState(mensaje, wasClosed);

            if (wasClosed) 
                break;

        } catch (const std::exception& e) {
            std::cerr << "Error sender: " << e.what() << std::endl;
            break;
        }
    }
    std::cout << "Sender\n";

}

void Sender::broadcast_message_with_info(game_state_t gameState) {
    if (gameStateQueue->isClosed()) {
        return;  
    }
    try {
        gameStateQueue->push(gameState);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Sender::stop() {
    if (!_keep_running) return;
    try {
        Thread::stop();
        game_state_t msg;
        while (gameStateQueue->try_pop(msg)) {}
        gameStateQueue->close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

Sender::~Sender() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}