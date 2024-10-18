#include "serverprotocol.h"


uint8_t Serverprotocol::recvMovement(bool &wasClosed){
    return recvUint8(wasClosed);
}

std::string Serverprotocol::recvPlayerName(bool &wasClosed){
    return deserializeString(wasClosed);
}

void Serverprotocol::sendFinalPosition(uint8_t *position ,bool &wasClosed) {
    sendUint8(position[0], wasClosed);
    sendUint8(position[1], wasClosed);
}
