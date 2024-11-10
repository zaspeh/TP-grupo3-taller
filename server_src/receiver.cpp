#include "receiver.h"

#include <iostream>

#include "server.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop, Monitor& monitor):
        server(server), protocol(protocol), gameLoop(gameLoop), monitor(monitor) {}

void Receiver::run() {
    bool wasClosed = false; 
    uint8_t idPlayer = 255;
    while (_keep_running && !wasClosed && server.esta_corriendo()) {
        try {
            std::vector<uint8_t> mensaje = protocol->recvCommand(wasClosed);
            if (wasClosed) {
                std::cout << "Saliendo del receiver.\n";
                break;
            }
            idPlayer = mensaje[0];
            
            std::cout << "Received command: " << std::to_string(mensaje[0]) << " " << std::to_string(mensaje[1]) << std::endl;
            server.obtener_gameloop().agregar_comando([this, mensaje]() {
                //std::cout << "Agregando comandos\n";
                gameLoop.doActionGameState(mensaje[0], mensaje[1]);
                //std::cout << "COmando agreagado\n";
            });
            
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " receiver - " << e.what() << std::endl;
            break;
        }
    }

    try {
        std::cout << "Eliminando jugador " << idPlayer << std::endl;
        server.removeClient(protocol);
        gameLoop.removePlayer(idPlayer); 
    } catch (const std::exception& e) {
        std::cerr << "Error al remover jugador: " << e.what() << std::endl;
    }
}