#include "receiver.h"

#include <iostream>

#include "server.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop) :
        server(server), protocol(protocol), gameLoop(gameLoop) {}

void Receiver::run() {
    bool wasClosed = false; 
    
    while (_keep_running && !wasClosed && server.esta_corriendo()) {
        try {
            std::vector<uint8_t> mensaje = protocol->recvCommand(wasClosed);
            if (wasClosed) {
                break;
            }
            std::cout << "Received command: " << std::to_string(mensaje[0]) << " " << std::to_string(mensaje[1]) << std::endl;
            server.obtener_gameloop().agregar_comando([this, mensaje]() {
                //if(mensaje[1] == START_MATCH) {
                if(!gameLoop.matchStarted) {
                    //gameLoop.initGame();
                    gameLoop.firstTime(mensaje[0]); // if is the first msg from that client, returns true
                    gameLoop.matchStarted = true;
                }
                gameLoop.doActionGameState(mensaje[0], mensaje[1]);
            });

            
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " receiver - " << e.what() << std::endl;
            break;
        }
    }

    server.removeClient(protocol);
}