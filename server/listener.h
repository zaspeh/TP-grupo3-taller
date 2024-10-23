#ifndef LISTENER_H
#define LISTENER_H

#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "receiver.h"
#include "sender.h"

class Server;
class Monitor;

class Listener: public Thread {
private:
    Socket socket_servidor;
    Server& server;
    Monitor& monitor;
    std::vector<std::shared_ptr<Sender>> emisores;
    std::vector<std::shared_ptr<Receiver>> receptores;


public:
    Listener(int port, Server& server, Monitor& monitor);

    // Devuelve el vector de emisores de los clientes.
    std::vector<std::shared_ptr<Sender>>& obtener_emisores() { return emisores; }

    // Ejecuta el hilo aceptador de conexiones.
    void run() override;

    // Detiene el hilo aceptador de conexiones.
    void stop() override;

    // Destruye el listener, liberanndo todos los recursos reservados.
    ~Listener();
};

#endif  // LISTENER_H