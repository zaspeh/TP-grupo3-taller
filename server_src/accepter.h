#ifndef ACCEPTER_H
#define ACCEPTER_H

#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include <arpa/inet.h>

// Own libraries
#include "receiver.h"
#include "sender.h"
#include "monitor.h"
#include "../common_src/utils.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "../common_src/serverprotocol.h"

class Server;

class Accepter: public Thread {
private:
    Socket socket_servidor;
    Server& server;
    Monitor& monitor;
    GameLoop & gameLoop;
    std::vector<std::shared_ptr<Sender>> emisores;
    std::vector<std::shared_ptr<Receiver>> receptores;


public:
    Accepter(int port, Server& server, Monitor& monitor, GameLoop& gameLoop);

    // Devuelve el vector de emisores de los clientes.
    std::vector<std::shared_ptr<Sender>>& obtener_emisores() { return emisores; }

    // Ejecuta el hilo aceptador de conexiones.
    void run() override;

    // Detiene el hilo aceptador de conexiones.
    void stop() override;

    // Destruye el Accepter, liberanndo todos los recursos reservados.
    ~Accepter();
};

#endif  // ACCEPTER_H