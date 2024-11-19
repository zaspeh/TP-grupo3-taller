#include "receiver.h"
#include <iostream>
#include "server.h"
#include "../common_src/utils.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop, Monitor& monitor):
        server(server), protocol(protocol), gameLoop(gameLoop), monitor(monitor) {}

void Receiver::run() {
    bool wasClosed = false; 
    uint8_t idPlayer = 255;
    while (_keep_running && !wasClosed) {
        try {
            std::vector<uint8_t> mensaje = protocol->recvCommand(wasClosed);
            if (wasClosed) 
                break;
            
            idPlayer = mensaje[0];
            
            gameLoop.addCommand([this, mensaje]() {
                gameLoop.doActionGameState(mensaje[0], mensaje[1]);
            });
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            break;
        }
    }

    try {
        server.removeClient(protocol);
        gameLoop.removePlayer(idPlayer); 
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}