#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common_src/caja.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "gameloop.h"
#include "listener.h"
#include "monitor.h"
#include "receiver.h"
#include "sender.h"

class Server: public Thread {
private:
    Listener listener;
    GameLoop gameloop;
    Monitor monitor;

    // Cierra todas las conexiones de clientes activos.
    void closeClients();

public:
    explicit Server(int port);

    // Ejecuta el servidor.
    void run() override;

    // Detiene el servidor y cierra todas las conexiones.
    void stop() override;

    // Agrega un cliente nuevo al servidor.
    void addClient(std::shared_ptr<Protocolo> client);

    // Elimina un cliente del servidor.
    void removeClient(std::shared_ptr<Protocolo> client);

    // Maneja la entrada del usuario desde la consola.
    void handleInput();

    // Devuelve el vector de clientes conectados.
    std::vector<std::shared_ptr<Protocolo>> obtener_clientes();

    // Devuelve una referencia al gameloop.
    GameLoop& obtener_gameloop() { return gameloop; }

    // Devuelve una referencia al monitor.
    Monitor& obtener_monitor() { return monitor; }

    // Verifica si el servidor está en ejecución.
    bool esta_corriendo() const { return _keep_running; }

    // Devuelve los Senders de los usuarios.
    std::vector<std::shared_ptr<Sender>>& obtener_emisores();

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Server();
};

#endif  // SERVER_H