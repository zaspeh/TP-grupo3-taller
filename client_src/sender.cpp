#include "sender.h"

/*
#define GRENADE_WEAPON 0x1
#define BANANA_WEAPON 0x2
#define PEWPEWLASER_WEAPON 0x3
#define LASERRIFLE_WEAPON 0x4
#define AK_47_WEAPON 0x5
#define DARTGUN_WEAPON 0x6
#define COWBOY_WEAPON 0x7
#define MAGNUM_WEAPON 0x8
#define SHOTGUN_WEAPON 0x9
#define SNIPER_WEAPON 0xa
*/
bool Sender::commandIsValid(uint8_t command) {
    return command == MOVE_LEFT || command == MOVE_RIGHT || command == JUMP || command == TAKE_WEAPON || command == SHOOT 
    || command == LOOK_UP || command == FLOOR || command == NEW_CLIENT || command == INFINIT_AMMO || command == PICK_ANY_WEAPON
    || command == GRENADE_WEAPON || command == BANANA_WEAPON || command == PEWPEWLASER_WEAPON || command == LASERRIFLE_WEAPON
    || command == AK_47_WEAPON || command == DARTGUN_WEAPON || command == COWBOY_WEAPON || command == MAGNUM_WEAPON
    || command == SHOTGUN_WEAPON || command == SNIPER_WEAPON || command == CHESTPLATE_ARMOR || command == HELMET_ARMOR || command == RESTART_MATCH;

}

void Sender::run() {
    bool wasClosed = false;
    while (!wasClosed && _keep_running) {
        try {
            //std::cout << "Popeando el mensaje" << std::endl;
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
    uint8_t command;
    while (commandQueue->try_pop(command)) {
    }
    commandQueue->close();
}

Sender::Sender(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<uint8_t>> queue) : protocol(protocol), commandQueue(queue) {}