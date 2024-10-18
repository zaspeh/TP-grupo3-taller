#include "protocol.h"

void Protocol::checkReceivedStatus(int receivedBytes, bool was_closed,
                                     const std::string& error_message) {
    if (receivedBytes <= 0 || was_closed) {
        std::cerr << error_message << std::endl;
        exit(1);
    }
}

std::string Protocol::deserializeString(bool& wasClosed) {
    uint16_t length = recvUint16( wasClosed);
    std::vector<char> buffer(length);
    int receivedBytes = this->socket.recvall(buffer.data(), length, &wasClosed);
    
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_STRING);

    return std::string(buffer.begin(), buffer.end());
}

void Protocol::sendString(const std::string& string, bool& wasClosed) {
    sendUint16(static_cast<uint16_t>(string.size()), wasClosed);
    this->socket.sendall(string.c_str(), string.size(), &wasClosed);
}

void Protocol::sendUint8(uint8_t num, bool& wasClosed) {
    uint8_t networkNum = htons(num);
    this->socket.sendall(reinterpret_cast<char*>(&networkNum), sizeof(networkNum), &wasClosed);
}

uint8_t Protocol::recvUint8(bool& wasClosed) {
    uint8_t num;
    int receivedBytes = this->socket.recvall(&num, sizeof(num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_INT);
    return num;
}

void Protocol::sendUint16(uint16_t num, bool& wasClosed) {
    uint16_t network_num = htons(num);
    this->socket.sendall(reinterpret_cast<char*>(&network_num), sizeof(network_num), &wasClosed);
}

uint16_t Protocol::recvUint16(bool& wasClosed) {
    uint16_t network_num;
    int receivedBytes = this->socket.recvall(&network_num, sizeof(network_num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_INT);
    return ntohs(network_num);
}

Protocol::Protocol(Socket& socket) : socket(socket) {}