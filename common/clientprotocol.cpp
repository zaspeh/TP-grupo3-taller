#include "clientprotocol.h"


void Clientprotocol::sendMovement(uint8_t num, bool &wasClosed){
    this->sendUint8(num, wasClosed);
}

void Clientprotocol::sendPlayerName(std::string playerName, bool &wasClosed){
    this->sendString(playerName, wasClosed);
}

uint8_t *Clientprotocol::recvFinalPosition(bool &wasClosed) {
    uint8_t buffer[2];
    buffer[0] = this->recvUint8(wasClosed);
    buffer[1] = this->recvUint8(wasClosed);
    return buffer;
}
