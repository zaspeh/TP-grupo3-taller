#ifndef MONITOR_H
#define MONITOR_H

#include <memory>
#include <mutex>
#include <vector>

#include "client.h"
#include "server.h"
#include "../common_src/serverprotocol.h"

class Client;
class Server;

class Monitor {
private:
    std::unique_ptr<Server> server;
    std::mutex mutex_clientes;
    std::mutex mutex_senders;

public:
    explicit Monitor(Server& server);

    // Eliminar el constructor de copia
    Monitor(const Monitor&) = delete;
    Monitor& operator=(const Monitor&) = delete;

    // Procesa un mensaje recibido.
    void procesar_mensaje(game_state_t gameState);


    // Métodos para manejar clientes

    // Agrega un nuevo cliente al monitor.
    void addToMap(std::map<uint8_t, std::shared_ptr<Client>>& map, uint8_t idClient, std::shared_ptr<Client> client);

    void removeFromMap(std::map<uint8_t, std::shared_ptr<Client>>& map, uint8_t idClient);

    // Cierra todas las conexiones de los clientes.
    void closeClients(std::map<uint8_t, std::shared_ptr<Client>>& clientes);
};

#endif  // MONITOR_H
