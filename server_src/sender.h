#ifndef SENDER_H
#define SENDER_H

#include <memory>
#include <string>
#include <unordered_map>

//#include "../common_src/informacion.h"
#include "../common_src/serverprotocol.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"

class Server;
class Monitor;

struct ClientMessage {
    std::vector<uint8_t> info;
};

class Sender: public Thread {
private:
    Server& server;
    Monitor& monitor;
    std::shared_ptr<ServerProtocol> protocol;
    std::shared_ptr<Queue<ClientMessage>> cola_mensajes;

public:
    explicit Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
                    std::shared_ptr<Queue<ClientMessage>> cola_mensajes);

    // Ejecuta el hilo sender.
    void run() override;

    // Envía un mensaje de reaparición de caja a todos los clientes conectados.
    void broadcast_message();

    // Envía un mensaje con información a todos los clientes conectados.
    void broadcast_message_with_info(const std::vector<uint8_t>& info);

    // Detiene el sender.
    void stop() override;

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Sender();
};

#endif  // SENDER_H