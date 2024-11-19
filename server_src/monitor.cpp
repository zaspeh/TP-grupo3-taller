#include "monitor.h"
#include <algorithm>
#include <iostream>

#include "server.h"

Monitor::Monitor(Server& server): server(server) {}

void Monitor::procesar_mensaje(const game_state_t gameState) {
    std::lock_guard<std::mutex> lock(mutex_senders);
    auto senders = server.obtener_emisores();
    if (!senders.empty()) {
        for (auto& sender : senders) {
            if (sender && !sender->isQueueClosed()) {  
                try {
                    sender->broadcast_message_with_info(gameState);
                } catch (const std::exception& e) {
                }
            }
        }
    }
}

void Monitor::agregar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.push_back(client);
}

void Monitor::eliminar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    try {
        client->closeSocket();
    
        auto it = std::remove(clientes.begin(), clientes.end(), client);
        if (it != clientes.end()) {
            clientes.erase(it, clientes.end());  
        }
        client = nullptr;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<std::shared_ptr<ServerProtocol>> Monitor::obtener_clientes() {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    return clientes;
}

void Monitor::cerrar_clientes() {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.clear();
}