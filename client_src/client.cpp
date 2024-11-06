#include "client.h"

std::atomic<uint16_t> Client::next_id(0);


void checkIfClose() {
    /* -> con la variable de game y 1 candado
    si se cierra...
    stop();
    */
}

Client::Client(const std::string& server_ip, const std::string& server_port)
        : socket(server_ip.c_str(), server_port.c_str()), // Asumimos que ClientProtocol tiene un constructor que acepta socket y client_id
          gameStateQueue(std::make_shared<Queue<game_state_t>>(100)), // Inicializa la cola de estados del juego
          commandQueue(std::make_shared<Queue<uint8_t>>(100)) // Inicializa la cola de comandos
    {
        requestId();
        clientprotocol = std::make_shared<ClientProtocol>(std::move(socket), client_id);

        sendThread = std::make_unique<Sender>(clientprotocol, commandQueue); 
        std::cout << "Sender: " << static_cast<int>(client_id) << std::endl;

        recvThread = std::make_unique<Receiver>(clientprotocol, gameStateQueue);
        std::cout << "REceiver: " << static_cast<int>(client_id) << std::endl;
        sendThread->start();


        gameThread = std::make_unique<Game>(gameStateQueue, commandQueue);

        std::cout << "Client ID: " << static_cast<int>(client_id) << std::endl;
    }

void Client::requestId() {
    unsigned int id_input;
    std::cout << "Ingrese el ID de cliente: ";
    std::cin >> id_input;

    // Asegurarse de que el ID esté en el rango de uint8_t
    if (id_input > 255) {
        std::cerr << "ID inválido. Debe estar en el rango [0, 255]." << std::endl;
        throw std::invalid_argument("ID fuera de rango");
    }

    client_id = static_cast<uint8_t>(id_input);
    std::cerr << "ID inválido. Debe estar en el rango [0, 255]." << std::endl;
    commandQueue->push(NEW_CLIENT); // para el servidor significa, 
    std::cerr << "ID inválido. Debe estar en el rango [0, 255]." << std::endl;
}

void Client::run() {
    recvThread->start();
    gameThread->start();

    gameThread->join();
    recvThread->join();
    sendThread->join();

    checkIfClose();
}


void Client::stop(){
    //monitor->cerrar_clientes();
    sendThread->stop();
    recvThread->stop();
    gameThread->stop();
}