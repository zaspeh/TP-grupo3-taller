#include "receiver.h"

void Receiver::run(){
    bool wasClosed = false;
    while (!wasClosed && _keep_running) {
        try {
            game_state_t state = protocol->readFromServer(wasClosed);
            if (wasClosed)
                break;
            std::cout << "Pusheamos el mensaje\n";
            std::cout << "Position: " << state.level.ducks[0].pos.x << " " << state.level.ducks[0].pos.y << std::endl;
            gameStateQueue->push(state);
            std::cout << "PusheaDISIMO\n";
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << e.what() << std::endl;
            break;
        }
    }
    //monitor.remove_client(protocol);
}

void Receiver::stop() {
    _keep_running = false;
}

Receiver::Receiver(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<game_state_t>> queue) : protocol(protocol), gameStateQueue(queue) {}