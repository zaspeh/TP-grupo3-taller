#include "receiver.h"
#include <iostream>
#include "server.h"
#include "../common_src/utils.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop, Monitor& monitor, int id):
        server(server), protocol(protocol), gameLoop(gameLoop), monitor(monitor), clientID(id) {}

void Receiver::run() {
    bool wasClosed = false; 
    uint8_t idPlayer;
    while (_keep_running && !wasClosed) {
        try {
            std::vector<uint8_t> mensaje = protocol->recvCommand(wasClosed);
            if (wasClosed) 
                break;
            
            if (mensaje[1] == NEW_CLIENT)
                idPlayer = clientID;
            else 
                idPlayer = mensaje[0];

            gameLoop.addCommand([this, idPlayer, mensaje]() {
                gameLoop.doActionGameState(idPlayer, mensaje[1]);
            });
            
        } catch (const std::exception& e) {
            //std::cerr << "Error receiver: " << e.what() << std::endl;
            break;
        }
    }

    try {
        server.removeClient(protocol);
    } catch (const std::exception& e) {
        //std::cerr << "Error: " << e.what() << std::endl;
    }
    //std::cout << "Saliendo del receiver\n";
}