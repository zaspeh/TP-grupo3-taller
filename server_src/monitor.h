// monitor.h
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

    // Devuelve true si la caja pedida está disponible.
    //bool verificar_disponibilidad_caja(const Informacion& info);

    // Procesa un mensaje recibido.
    void procesar_mensaje(const std::vector<uint8_t>& info_recibida);

    // Notifica a todos los clientes de la reaparición de una caja.
    //void notificar_reaparicion_caja();

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