
#include <iostream>
#include <exception>

//#include <SDL2pp/SDL2pp.hh>
//#include <SDL2/SDL.h>

// Mati
#include <sstream>
#include <string>
// Own libraries
#include "../common_src/clientprotocol.h"
#include "../common_src/socket.h"
#include "../common_src/utils.h"

const std::string CLIENT_QUIT = "quit"; // o algo similar

//using namespace SDL2pp;
/*
int main() try {
	// Initialize SDL library
	SDL sdl(SDL_INIT_VIDEO);

	// Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
	Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_RESIZABLE);

	// Create accelerated video renderer with default driver
	Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Clear screen
	renderer.Clear();

	// Show rendered frame
	renderer.Present();

	// 5 second delay
	SDL_Delay(5000);

	// Here all resources are automatically released and library deinitialized
	return 0;
} catch (std::exception& e) {
	// If case of error, print it and exit with error
	std::cerr << e.what() << std::endl;
	return 1;
}
*/

class Client {
	private:
		Socket socket;
		uint8_t client_id;
		static uint8_t next_id;
		ClientProtocol clientprotocol;

	void sendControl(std::string command, bool&wasClosed){
		if (command == "space") {
			clientprotocol.sendControlUsed(JUMP, wasClosed);
		} else if (command == "d") {
			clientprotocol.sendControlUsed(MOVE_RIGHT, wasClosed);
		} else if (command == "a") {
			clientprotocol.sendControlUsed(MOVE_LEFT, wasClosed);
		} else {
			std::cout << "There was a problem trying to traduce the command" << std::endl;
		}
	}

	void analyze_request(std::string request, bool& wasClosed) {
        std::stringstream arg(request);
        std::string command;
        arg >> command;

		if(command == "Read"){
			std::vector<uint8_t>  response = clientprotocol.recvFinalPosition(wasClosed);
			std::cout << "Response1: " << static_cast<int>(response[0]) << std::endl;
			std::cout << "Response2: " << static_cast<int>(response[1]) << std::endl;

		} else {
			this->sendControl(command, wasClosed);
		}

    }

	public:
    	Client(uint16_t server_port, const std::string& server_ip)
			: socket(server_ip.c_str(), std::to_string(server_port).c_str()),
			client_id(next_id++),
			clientprotocol(std::move(socket), client_id) {} // Inicialización correcta


		void run(){
			std::string request; // Añadir esta línea al principio de run()
			bool wasClosed = false;
			while (!wasClosed) {
				std::getline(std::cin, request);
				if (request == CLIENT_QUIT)
					break;

				analyze_request(request, wasClosed);
			}
		}
};

uint8_t Client::next_id = 1;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << CLIENT_USAGE_COMMAND_ERROR << std::endl;
        return -1;
    }

    Client client(atoi(argv[2]), argv[1]);
    client.run();

    return 0;
}