#include "sender.h"


bool Sender::commandIsValid(uint8_t command) {
    return command == MOVE_LEFT || command == MOVE_RIGHT || command == JUMP || command == TAKE_WEAPON || command == SHOOT || command == LOOK_UP || command == FLOOR;
}

void Sender::sendCommand() {
    bool wasClosed = false;
    while (!wasClosed && running.load(std::memory_order_acquire)) {
        try {
            uint8_t command = comandQueue.pop();
            if (wasClosed)
                break;

            if (commandIsValid(command))
                protocol.sendCommand(command, wasClosed);

        } catch (const std::exception& e) {
            break;
        }
    }
}

void Sender::Sender(ClientProtocol& protocol) : protocol(protocol) {}