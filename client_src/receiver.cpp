#include "receiver.h"

Receiver::Receiver(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<game_state_t>> queue) : protocol(protocol), gameStateQueue(queue) {}

void Receiver::readID(bool &wasClosed) {
    game_state_t state = protocol->readFromServer(wasClosed);
    protocol->setID(state.level.num_ducks - 1); // Con valgrind quitar el -1
    gameStateQueue->push(state);
}

void Receiver::run(){
    bool wasClosed = false;

    readID(wasClosed);
    while (!wasClosed && _keep_running) {
        try {
            game_state_t state = protocol->readFromServer(wasClosed);
            if (wasClosed)
                break;

            gameStateQueue->push(state);

        } catch (const std::exception& e) {
            break;
        }
    }
}

void Receiver::stop() {
    if (!_keep_running) return;
    _keep_running = false;
    protocol->closeSocket();
}

Receiver::~Receiver() {
    stop();
}