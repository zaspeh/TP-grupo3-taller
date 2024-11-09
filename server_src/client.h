#ifndef CLIENT_H
#define CLIENT_H

#include <memory>
#include "sender.h"
#include "server.h"
#include "receiver.h"
#include "../common_src/serverprotocol.h"

class Receiver;
class Sender;
class Monitor;
class GameLoop;

class Client: public std::enable_shared_from_this<Client>  {
private:
    Server& server;
    std::shared_ptr<Monitor> monitor;
    std::shared_ptr<GameLoop> gameLoop;
    std::shared_ptr<ServerProtocol> protocol;
    std::shared_ptr<Queue<game_state_t>> gameStateQueue;
    std::unique_ptr<Sender> sender;  
    std::unique_ptr<Receiver> receiver; 
    bool wasClosed;
    uint8_t idPlayer;

public:
    explicit Client(Server& server, std::shared_ptr<Monitor> monitor, std::shared_ptr<GameLoop> gameLoop, std::shared_ptr<ServerProtocol> protocol, uint8_t id);

    game_state_t getGameState() const;

    void sendGameState(game_state_t& game);

    void broadcast_message_with_info(game_state_t gameState);

    std::vector<uint8_t> recvCommand();

    void addCommand(std::vector<uint8_t>& mensaje);

    // Inicia el cliente (sus hilos Sender y Receiver).
    void start();

    // Detiene y limpia los recursos del cliente.
    void stop();

    // Verifica si la conexión del cliente está activa.
    bool isActive() const;

    ~Client();
};

#endif  // CLIENT_H
