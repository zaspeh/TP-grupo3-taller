#include "client.h"

Client::Client(Server& server, std::shared_ptr<Monitor> monitor, std::shared_ptr<GameLoop> gameLoop, std::shared_ptr<ServerProtocol> protocol, uint8_t id)
    : server(server), 
      monitor(monitor), 
      gameLoop(gameLoop), 
      protocol(std::move(protocol)), 
      gameStateQueue(std::make_shared<Queue<game_state_t>>(MAX_CLIENTS_PER_QUEUE)),
      wasClosed(false), 
      idPlayer(id) {
    sender = std::make_unique<Sender>(server, shared_from_this());
    receiver = std::make_unique<Receiver>(server, shared_from_this());;
}

void Client::start() {
    sender->start();
    receiver->start();
}

void Client::stop() {
    std::cout << "Eliminando cliente" << std::endl;
    protocol->closeSocket();
    gameLoop->removePlayer(idPlayer);
    server.removeClient(idPlayer); 
    game_state_t state;
    while (gameStateQueue->try_pop(state)) {}
    gameStateQueue->close();
    sender->stop();
    receiver->stop();

    sender->join();
    receiver->join();
}

bool Client::isActive() const {
    return protocol || !gameStateQueue->isClosed();
}

game_state_t Client::getGameState() const {
    if (!isActive()) {
        return {};
    }
    return gameStateQueue->pop();
}

void Client::sendGameState(game_state_t& game) {
    if (!isActive()) {
        stop();
    }
    protocol->sendGameState(game, wasClosed);
    if (wasClosed) {
        stop();
    }
}

void Client::broadcast_message_with_info(game_state_t gameState) {
    std::cout << "Broadcasting message with info" << std::endl;
    if (!isActive()) {
        stop();
    }
    gameStateQueue->push(gameState);
    std::cout << "Message broadcasted with info" << std::endl;
}

std::vector<uint8_t> Client::recvCommand() {
    if (!isActive()) {
        stop();
    }
    std::vector<uint8_t> mensaje = protocol->recvCommand(wasClosed);
    if (wasClosed) {
        stop();
    }
    return mensaje;
}

void Client::addCommand(std::vector<uint8_t>& mensaje) {
    gameLoop->agregar_comando([this, mensaje]() {
        std::cout << "Agregando comandos\n";
        gameLoop->doActionGameState(mensaje[0], mensaje[1]);
        std::cout << "COmando agreagado\n";
    });
}

Client::~Client() {
    stop();
}
