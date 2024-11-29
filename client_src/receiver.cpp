#include "receiver.h"

Receiver::Receiver(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<game_state_t>> queue) : protocol(protocol), gameStateQueue(queue) {}

void Receiver::readID(bool &wasClosed) {
    game_state_t state = protocol->readFromServer(wasClosed);
    std::cout << "ID actualizado: " << static_cast<int>(state.level.num_ducks) << std::endl;
    protocol->setID(state.level.num_ducks); // Sin valgrind agregar -1
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

        /* for (int i = 0; i < state.level.num_ducks; ++i) {
            const duck_t& duck = state.level.ducks[i];
            std::cout << "Pato ID: " << static_cast<int>(duck.id) 
                    << ", Posición: (" << duck.pos.x << ", " << duck.pos.y << ")"
                    << std::endl;
        } */

            gameStateQueue->push(state);

        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            break;
        }
    }
}

void Receiver::stop() {
    if (!_keep_running) return;
    _keep_running = false;
    protocol->closeSocket();
    std::cout << "Receiver stopped" << std::endl;
}

Receiver::~Receiver() {
    stop();
}