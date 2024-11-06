#include "receiver.h"

#include <iostream>

#include "server.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop, Monitor& monitor):
        server(server), protocol(protocol), gameLoop(gameLoop), monitor(monitor) {}

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
                gameLoop.doActionGameState(mensaje[0], mensaje[1]);
            });

            
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " receiver - " << e.what() << std::endl;
            break;
        }
    }

    server.removeClient(protocol);
}