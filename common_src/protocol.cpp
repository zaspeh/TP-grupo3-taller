#include "protocol.h"

#include <cstring> // Para usar memcpy

void Protocol::checkReceivedStatus(int receivedBytes, bool was_closed,
                                     const std::string& error_message) {
    if (receivedBytes <= 0 || was_closed) {
        //std::cerr << error_message << std::endl;
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
    this->socket.sendall(reinterpret_cast<char*>(&num), sizeof(num), &wasClosed);
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

void Protocol::sendUint32(uint32_t num, bool& wasClosed) {
    uint32_t network_num = htons(num);
    this->socket.sendall(reinterpret_cast<char*>(&network_num), sizeof(network_num), &wasClosed);
}

uint32_t Protocol::recvUint32(bool& wasClosed) {
    uint32_t network_num;
    int receivedBytes = this->socket.recvall(&network_num, sizeof(network_num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_INT);
    return ntohs(network_num);
}

void Protocol::sendInt(int num, bool& wasClosed) {
    int network_num = htonl(num);
    this->socket.sendall(reinterpret_cast<char*>(&network_num), sizeof(network_num), &wasClosed);
}

int Protocol::recvInt(bool& wasClosed) {
    int network_num;
    int receivedBytes = this->socket.recvall(&network_num, sizeof(network_num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_INT);
    return ntohl(network_num);
}


Protocol::Protocol(Socket socket) : socket(std::move(socket)) {}

void Protocol::closeSocket() {
    this->socket.shutdown(SHUT_RDWR);
    this->socket.close();
}
/*
float Protocol::recvFloat(bool& wasClosed) {
    uint32_t network_num;
    int receivedBytes = this->socket.recvall(&network_num, sizeof(network_num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_FLOAT);
    network_num = ntohl(network_num); // Convierte de formato de red a formato de host
    
    float num;
    std::memcpy(&num, &network_num, sizeof(float)); // Copia los bytes al float
    return num;
}

void Protocol::sendFloat(float num, bool& wasClosed) {
    uint32_t network_num;
    std::memcpy(&network_num, &num, sizeof(float)); // Copia los bytes del float a un uint32_t
    network_num = htonl(network_num); // Convierte a formato de red
    this->socket.sendall(reinterpret_cast<char*>(&network_num), sizeof(network_num), &wasClosed);
}
*/



void Protocol::sendFloat(float num, bool& wasClosed) {
    uint32_t network_num;
    std::memcpy(&network_num, &num, sizeof(float));
    network_num = htonl(network_num); 
    this->socket.sendall(reinterpret_cast<char*>(&network_num), sizeof(network_num), &wasClosed);
}

float Protocol::recvFloat(bool& wasClosed) {
    uint32_t network_num;
    int receivedBytes = this->socket.recvall(&network_num, sizeof(network_num), &wasClosed);
    checkReceivedStatus(receivedBytes, wasClosed, ERROR_READING_FLOAT);
    network_num = ntohl(network_num); 
    
    float num;
    std::memcpy(&num, &network_num, sizeof(float)); 
    return num;
}

/* float Protocol::recvFloat(bool &wasClosed) {
    // Recibir bytes
    uint8_t bytes[sizeof(float)];
    for (size_t i = 0; i < sizeof(float); i++) {
        bytes[i] = recvUint8(wasClosed);
        if (wasClosed) return 0.0f;
    }
    
    // Convertir bytes a float
    float value;
    memcpy(&value, bytes, sizeof(float));
    return value;
}

void Protocol::sendFloat(float value, bool &wasClosed) {
    // Convertir float a bytes
    uint8_t bytes[sizeof(float)];
    memcpy(bytes, &value, sizeof(float));
    
    // Enviar cada byte
    for (size_t i = 0; i < sizeof(float); i++) {
        sendUint8(bytes[i], wasClosed);
        if (wasClosed) return;
    }
} */
