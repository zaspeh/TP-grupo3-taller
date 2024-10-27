#include "serverprotocol.h"

ServerProtocol::ServerProtocol(Socket socket) : Protocol(std::move(socket)) {}

std::vector<uint8_t> ServerProtocol::recvMovement(bool &wasClosed){
    std::vector<uint8_t> buffer(2);
    buffer[0] = recvUint8(wasClosed);
    buffer[1] = recvUint8(wasClosed);
    return buffer;
}

void ServerProtocol::sendFinalPosition(std::vector<uint8_t> position ,bool &wasClosed) {
    sendUint8(position[0], wasClosed);
    sendUint8(position[1], wasClosed);
}
