#include "clientprotocol.h"

void ClientProtocol::sendControlUsed(uint8_t num, bool &wasClosed) {
    this->sendUint8(this->client_identifier, wasClosed);
    this->sendUint8(num, wasClosed);
}

std::vector<uint8_t> ClientProtocol::recvFinalPosition(bool &wasClosed) {
    std::vector<uint8_t> buffer(2);
    buffer[0] = this->recvUint8(wasClosed);
    buffer[1] = this->recvUint8(wasClosed);
    return buffer;
}

uint8_t ClientProtocol::readFromServer(bool &wasClosed) {
    return this->recvUint8(wasClosed);
}


ClientProtocol::ClientProtocol(Socket socket, uint8_t client_id)
        : Protocol(std::move(socket)),  
          client_identifier(client_id) {}  