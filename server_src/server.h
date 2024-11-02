#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>
// Own libraries
#include "../common_src/socket.h"
#include "../common_src/serverprotocol.h"
#include "../common_src/thread.h"

#include "accepter.h"
#include "gameloop.h"
#include "monitor.h"
#include "receiver.h"
#include "sender.h"
#include "game_state.h"

class Server: public Thread {
private:
    GameLoop gameloop;
    Monitor monitor;
    Accepter accepter;

    // Cierra todas las conexiones de clientes activos.
    void closeClients();

public:
    explicit Server(int port);

    // Ejecuta el servidor.
    void run() override;

    // Detiene el servidor y cierra todas las conexiones.
    void stop() override;

    // Agrega un cliente nuevo al servidor.
    void addClient(std::shared_ptr<ServerProtocol> client);

    // Elimina un cliente del servidor.
    void removeClient(std::shared_ptr<ServerProtocol> client);

    // Maneja la entrada del usuario desde la consola.
    void handleInput();

    // Devuelve el vector de clientes conectados.
    std::vector<std::shared_ptr<ServerProtocol>> getClients();

    // Devuelve una referencia al gameloop.
    GameLoop& obtener_gameloop() { return gameloop; }

    // Devuelve una referencia al monitor.
    Monitor& obtener_monitor() { return monitor; }

    // Verifica si el servidor está en ejecución.
    bool esta_corriendo() const { return _keep_running; }

    // Devuelve los Senders de los usuarios.
    std::vector<std::shared_ptr<Sender>>& obtener_emisores();

    void doActionGameState(uint8_t id, uint8_t action);

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Server();
};

#endif  // SERVER_H