#ifndef MONITOR_H
#define MONITOR_H

#include <memory>
#include <mutex>
#include <vector>

#include "sender.h"
#include "../common_src/serverprotocol.h"

class Monitor {
private:
    Server& server;
    std::mutex mutex_clientes;
    std::mutex mutex_senders;
    std::vector<std::shared_ptr<ServerProtocol>> clientes;

public:
    explicit Monitor(Server& server);

    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;

    void procesar_mensaje(game_state_t gameState);
    void agregar_cliente(std::shared_ptr<ServerProtocol> client);
    void eliminar_cliente(std::shared_ptr<ServerProtocol> client);
    std::vector<std::shared_ptr<ServerProtocol>> obtener_clientes();


    void cerrar_clientes();
};

#endif  // MONITOR_H
