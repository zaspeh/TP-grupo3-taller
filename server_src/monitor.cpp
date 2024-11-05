#include "monitor.h"

#include <iostream>

#include "server.h"

Monitor::Monitor(Server& server): server(server) {}

/* bool Monitor::verificar_disponibilidad_caja(const Informacion& info) {
    std::lock_guard<std::mutex> lock(mutex_cajas);
    auto& cajas = server.obtener_cajas();
    CajaID id_caja = static_cast<CajaID>(info.obtener_id_caja());
    if (cajas.find(id_caja) != cajas.end() &&
        cajas[id_caja].esta_disponible(server.obtener_gameloop().obtener_iteraciones())) {
        cajas[id_caja].cambiar_ultima_iteracion(server.obtener_gameloop().obtener_iteraciones());
        cajas[id_caja].actualizar_estado();
        return true;
    }
    return false;
} */

void Monitor::procesar_mensaje(const game_state_t gameState) {
    game_state_t copia = gameState;
    //info_copia.establecer_id_recompensa(server.obtener_cajas());
    //info_copia.imprimir_informacion_servidor();
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
}

/* void Monitor::notificar_reaparicion_caja() {
    std::lock_guard<std::mutex> lock(mutex_senders);
    auto senders = server.obtener_emisores();
    if (!senders.empty()) {
        for (auto& sender: senders) {
            if (sender) {
                sender->broadcast_message();
            }
        }
        std::cout << REAPARECIO_CAJA << std::endl;
    }
} */

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