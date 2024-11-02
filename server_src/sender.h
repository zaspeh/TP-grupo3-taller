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

class Sender: public Thread {
private:
    Server& server;
    Monitor& monitor;
    std::shared_ptr<ServerProtocol> protocol;
    std::shared_ptr<Queue<game_state_t>> gameStateQueue;

public:
    explicit Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
                    std::shared_ptr<Queue<game_state_t>> gameStateQueue);

    // Ejecuta el hilo sender.
    void run() override;

    // Envía un mensaje con información a todos los clientes conectados.
    void broadcast_message_with_info(game_state_t gameState);

    // Detiene el sender.
    void stop() override;

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Sender();
};

#endif  // SENDER_H