#include "sender.h"
#include <unordered_set>

bool Sender::commandIsValid(uint8_t command) {
    static const std::unordered_set<uint8_t> validCommands = {
        MOVE_LEFT, MOVE_RIGHT, JUMP, TAKE_WEAPON, SHOOT,
        LOOK_UP, FLOOR, NEW_CLIENT, INFINIT_AMMO, PICK_ANY_WEAPON,
        GRENADE_WEAPON, BANANA_WEAPON, PEWPEWLASER_WEAPON, LASERRIFLE_WEAPON,
        AK_47_WEAPON, DARTGUN_WEAPON, COWBOY_WEAPON, MAGNUM_WEAPON,
        SHOTGUN_WEAPON, SNIPER_WEAPON, CHESTPLATE_ARMOR, HELMET_ARMOR,
        RESTART_MATCH, LEAVE_MATCH, YELLOW_DUCK, GREY_DUCK, ORANGE_DUCK, WHITE_DUCK
    };

    return validCommands.find(command) != validCommands.end();
}


void Sender::run() {
    bool wasClosed = false;
    while (!wasClosed && _keep_running) {
        try {
            uint8_t command = commandQueue->pop();

            if (wasClosed)
                break;
            if (commandIsValid(command))
                protocol->sendCommand(command, wasClosed);

        } catch (const std::exception& e) {
            break;
        }
    }
    std::cout << "Saliendo del sender\n";
    /* if (!wasClosed) {
        protocol->sendCommand(LEAVE_MATCH, wasClosed);
        std::cout << "Enviando comando de desconexión\n";
    } */
}

void Sender::stop() {
    _keep_running = false;
    uint8_t command;
    while (commandQueue->try_pop(command)) {
    }
    commandQueue->close();
}

Sender::Sender(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<uint8_t>> queue) : protocol(protocol), commandQueue(queue) {}