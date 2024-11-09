#include "monitor.h"
#include <algorithm>
#include <iostream>

#include "server.h"

Monitor::Monitor(Server& server): server(server) {}

void Monitor::procesar_mensaje(const game_state_t gameState) {
    std::cout << "Intentando tomar el lock en procesar_mensaje" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_senders);
    std::cout << "Lock tomado" << std::endl;
    auto senders = server.obtener_emisores();
    if (!senders.empty()) {
        for (auto& sender : senders) {
            std::cout << "Enviando mensaje" << std::endl;
            if (sender && !sender->isQueueClosed()) {  // Verifica si la cola está abierta
                try {
                    sender->broadcast_message_with_info(gameState);
                    std::cout << "Mensaje enviado" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error: fallo al enviar mensaje - " << e.what() << std::endl;
                }
            }
        }
    }
}


/* void Monitor::removeSender(uint8_t idClient) {
    std::lock_guard<std::mutex> lock(mutex_senders);
    auto& emisores = server.obtener_emisores();
    
    if (idClient < emisores.size() && emisores[idClient]) {  // Verifica que el índice es válido
        emisores[idClient]->stop();  // Cierra la cola antes de eliminar
        emisores.erase(emisores.begin() + idClient);  // Elimina el sender
    }
} */



void Monitor::agregar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.push_back(client);
}

void Monitor::eliminar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);

    auto it = std::remove(clientes.begin(), clientes.end(), client);
    if (it != clientes.end()) {
        clientes.erase(it, clientes.end());  
    }
    client = nullptr;
}



std::vector<std::shared_ptr<ServerProtocol>> Monitor::obtener_clientes() {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    return clientes;
}

void Monitor::cerrar_clientes() {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    for (auto& client: clientes) {
        client->closeSocket();
    }
    clientes.clear();
}