#include "client.h"

uint8_t Client::next_id = 1;

void checkIfClose() {
    /* -> con la variable de game y 1 candado
    si se cierra...
    stop();
    */
}

Client::Client(int server_port, const std::string& server_ip)
        : socket(server_ip.c_str(), std::to_string(server_port).c_str()),
          client_id(next_id++),
          clientprotocol(std::make_shared<ClientProtocol>(std::move(socket), client_id)), // Asumimos que ClientProtocol tiene un constructor que acepta socket y client_id
          gameStateQueue(std::make_shared<Queue<game_state_t>>(100)), // Inicializa la cola de estados del juego
          commandQueue(std::make_shared<Queue<uint8_t>>(100)), // Inicializa la cola de comandos
          sendThread(clientprotocol, commandQueue), // Inicializa el hilo de envío
          recvThread(clientprotocol, gameStateQueue), // Inicializa el hilo de recepción
          gameThread(gameStateQueue, commandQueue) // Inicializa el hilo del juego
    {}


void Client::run() {
    gameThread.start();
    recvThread.start();
    gameThread.start();

    gameThread.join();
    recvThread.join();
    gameThread.join();

    checkIfClose();
}


void Client::stop(){
    //monitor.cerrar_clientes();
    sendThread.stop();
    recvThread.stop();
    gameThread.stop();
}