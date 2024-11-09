#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>
#include <map>
#include "gameloop.h"
#include "game_state.h"
#include "../common_src/thread.h"

class Accepter;
class Monitor;
class Client;
class GameLoop;

class Server: public Thread {
private:
    std::shared_ptr<GameLoop> gameloop;  // Cambiar a shared_ptr
    std::shared_ptr<Monitor> monitor;
    std::shared_ptr<Accepter> accepter;  // Cambiar a unique_ptr
    std::map<uint8_t, std::shared_ptr<Client>> clients;

    // Cierra todas las conexiones de clientes activos.
    void closeClients();

public:
    explicit Server(int port);

    // Ejecuta el servidor.
    void run() override;

    // Detiene el servidor y cierra todas las conexiones.
    void stop() override;

    // Agrega un cliente nuevo al servidor.
    void addClient(uint8_t idClient, std::shared_ptr<Client> client);

    // Elimina un cliente del servidor.
    void removeClient(uint8_t idClient);

    // Maneja la entrada del usuario desde la consola.
    void handleInput();

    // Verifica si el servidor está en ejecución.
    bool esta_corriendo() const { return _keep_running; }

    // Devuelve los Senders de los usuarios.
    std::map<uint8_t, std::shared_ptr<Client>>& getClients();

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Server();
};

#endif  // SERVER_H