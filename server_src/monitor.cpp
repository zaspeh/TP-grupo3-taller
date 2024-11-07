#include "monitor.h"

#include <iostream>

#include "server.h"

Monitor::Monitor(Server& server): server(server) {}

void Monitor::procesar_mensaje(const game_state_t gameState) {
    game_state_t copia = gameState;
    //info_copia.establecer_id_recompensa(server.obtener_cajas());
    //info_copia.imprimir_informacion_servidor();
    std::cout << "Estoy por enviar la informacion al cliente" << std::endl;
    std::lock_guard<std::mutex> lock(mutex_senders);
    auto senders = server.obtener_emisores();
    //std::cout << "Posición pato: " << gameState.level.ducks[0].pos.x << " " << gameState.level.ducks[0].pos.y << std::endl;
    if (!senders.empty()) {
        for (auto& sender: senders) {
            if (sender) {
                sender->broadcast_message_with_info(copia);
            }
        }
    }
    std::cout << "Envio la informacion al cliente" << std::endl;
}

void Monitor::agregar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.push_back(client);
}

void Monitor::eliminar_cliente(std::shared_ptr<ServerProtocol> client) {
    std::lock_guard<std::mutex> lock(mutex_clientes);
    clientes.erase(std::remove(clientes.begin(), clientes.end(), client), clientes.end());
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