#include "sender.h"


bool Sender::commandIsValid(uint8_t command) {
    return command == MOVE_LEFT || command == MOVE_RIGHT || command == JUMP || command == TAKE_WEAPON || command == SHOOT || command == LOOK_UP || command == FLOOR || command == NEW_CLIENT;
}

void Sender::run() {
    bool wasClosed = false;
    while (!wasClosed && _keep_running) {
        try {
            std::cout << "Popeando el mensaje" << std::endl;
            uint8_t command = commandQueue->pop();

            if (wasClosed)
                break;
            std::cout << "Comando: " << static_cast<int>(command) << std::endl;
            if (commandIsValid(command))
                protocol->sendCommand(command, wasClosed);

        } catch (const std::exception& e) {
            break;
        }
    }
}

void Sender::stop() {
    _keep_running = false;
}

Sender::Sender(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<uint8_t>> queue) : protocol(protocol), commandQueue(queue) {}