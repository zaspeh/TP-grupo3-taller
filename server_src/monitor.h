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
    std::mutex mutex_cajas;
    std::vector<std::shared_ptr<ServerProtocol>> clientes;

public:
    explicit Monitor(Server& server);

    // Eliminar el constructor de copia
    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;

    // Procesa un mensaje recibido.
    void procesar_mensaje(game_state_t gameState);


    // Métodos para manejar clientes

    // Agrega un nuevo cliente al monitor.
    void agregar_cliente(std::shared_ptr<ServerProtocol> client);

    // Elimina un cliente del monitor.
    void eliminar_cliente(std::shared_ptr<ServerProtocol> client);

    // Devuelve una lista de todos los clientes actuales.
    std::vector<std::shared_ptr<ServerProtocol>> obtener_clientes();

    // Cierra todas las conexiones de los clientes.
    void cerrar_clientes();
};

#endif  // MONITOR_H
