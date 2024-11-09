#include "monitor.h"
#include <algorithm>
#include <iostream>

#include "server.h"

Monitor::Monitor(Server& server): server(std::unique_ptr<Server>(&server)) {}

void Monitor::procesar_mensaje(const game_state_t gameState) {
    std::cout << "Intentando tomar el lock en procesar_mensaje" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_clientes);
    std::cout << "Lock tomado" << std::endl;

    auto clients = server->getClients();  // Obtiene el mapa de clientes del servidor.
    if (!clients.empty()) {
        for (auto& [idClient, client] : clients) {
            if (client && client->isActive()) {  // Verifica si el cliente es válido y está activo.
                std::cout << "Enviando mensaje al cliente con ID: " << static_cast<int>(idClient) << std::endl;
                try {
                    client->broadcast_message_with_info(gameState);
                    std::cout << "Mensaje enviado al cliente con ID: " << static_cast<int>(idClient) << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error: fallo al enviar mensaje al cliente con ID " 
                              << static_cast<int>(idClient) << " - " << e.what() << std::endl;
                }
            } else {
                std::cerr << "Cliente con ID " << static_cast<int>(idClient) 
                          << " no está activo o es nulo." << std::endl;
            }
        }
    } else {
        std::cerr << "No hay clientes conectados." << std::endl;
    }
}

void Monitor::addToMap(std::map<uint8_t, std::shared_ptr<Client>>& clients, uint8_t idClient, std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clients[idClient] = client;
}

void Monitor::removeFromMap(std::map<uint8_t, std::shared_ptr<Client>>& clientes, uint8_t idClient) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.erase(idClient);
}

void Monitor::closeClients(std::map<uint8_t, std::shared_ptr<Client>>& clientes) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    for (auto& client : clientes) {
        client.second->stop();
    }
    clientes.clear();
}