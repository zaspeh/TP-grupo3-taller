#include "client.h"

std::atomic<uint16_t> Client::next_id(0);

Client::Client(const std::string& server_ip, const std::string& server_port)
        : socket(server_ip.c_str(), server_port.c_str()),
          gameStateQueue(std::make_shared<Queue<game_state_t>>(100)), 
          commandQueue(std::make_shared<Queue<uint8_t>>(100)), _keep_running(true)
    {
        requestId();
        clientprotocol = std::make_shared<ClientProtocol>(std::move(socket), client_id);

        recvThread = std::make_unique<Receiver>(clientprotocol, gameStateQueue);
        recvThread->start();
        
        sendThread = std::make_unique<Sender>(clientprotocol, commandQueue); 
        sendThread->start();


        gameThread = std::make_unique<Game>(gameStateQueue, commandQueue, *this);
        gameThread->start();
    }

void Client::checkIfClose() {
    std::cout << "Ingrese 'q' para cerrar el juego: ";

    while (_keep_running) {
        if (std::cin.rdbuf()->in_avail() > 0) {
            std::string input;
            std::getline(std::cin, input);
            if (input == "q") {
                break;
            }
            std::cout << "Entrada inválida. Intente nuevamente: ";
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    std::cout << "Saliendo de checkIfClose" << std::endl;
    stop();
}


/*
==28448== Invalid read of size 8
==28448==    at 0x54FBF37: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F76E: TTF_CloseFont (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x128E41: std::unique_ptr<_TTF_Font, void (*)(_TTF_Font*)>::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x126BC3: Game::~Game() (game.cpp:503)
==28448==    by 0x126D35: Game::~Game() (game.cpp:503)
==28448==    by 0x11884F: std::default_delete<Game>::operator()(Game*) const (unique_ptr.h:85)
==28448==    by 0x117835: std::unique_ptr<Game, std::default_delete<Game> >::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x116CC4: Client::~Client() (client.cpp:85)
==28448==    by 0x133D31: main (main.cpp:14)
==28448==  Address 0x1534cfe0 is 176 bytes inside a block of size 1,472 free'd
==28448==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==28448==    by 0x54FBFBF: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FDDC8: FT_Done_Library (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FDE81: FT_Done_FreeType (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A412CF: TTF_Quit (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x126A2E: Game::stop() (game.cpp:487)
==28448==    by 0x116B4B: Client::stop() (client.cpp:66)
==28448==    by 0x124A18: Game::run() (game.cpp:242)
==28448==    by 0x1276E9: Thread::main() (thread.h:43)
==28448==    by 0x133907: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==28448==    by 0x13385A: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==28448==    by 0x1337BA: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==28448==  Block was alloc'd at
==28448==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==28448==    by 0x54F6868: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54F6E9F: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FC24D: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F93E: TTF_OpenFontIndexDPIRW (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x123C6E: Game::loadMedia() (game.cpp:30)
==28448==    by 0x124774: Game::run() (game.cpp:200)
==28448==    by 0x1276E9: Thread::main() (thread.h:43)
==28448==    by 0x133907: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==28448==    by 0x13385A: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==28448==    by 0x1337BA: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==28448==    by 0x13376F: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==28448== 
==28448== Invalid read of size 8
==28448==    at 0x54FBF46: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F76E: TTF_CloseFont (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x128E41: std::unique_ptr<_TTF_Font, void (*)(_TTF_Font*)>::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x126BC3: Game::~Game() (game.cpp:503)
==28448==    by 0x126D35: Game::~Game() (game.cpp:503)
==28448==    by 0x11884F: std::default_delete<Game>::operator()(Game*) const (unique_ptr.h:85)
==28448==    by 0x117835: std::unique_ptr<Game, std::default_delete<Game> >::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x116CC4: Client::~Client() (client.cpp:85)
==28448==    by 0x133D31: main (main.cpp:14)
==28448==  Address 0x1534d020 is 240 bytes inside a block of size 1,472 free'd
==28448==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==28448==    by 0x54FBFBF: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FDDC8: FT_Done_Library (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FDE81: FT_Done_FreeType (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A412CF: TTF_Quit (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x126A2E: Game::stop() (game.cpp:487)
==28448==    by 0x116B4B: Client::stop() (client.cpp:66)
==28448==    by 0x124A18: Game::run() (game.cpp:242)
==28448==    by 0x1276E9: Thread::main() (thread.h:43)
==28448==    by 0x133907: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==28448==    by 0x13385A: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==28448==    by 0x1337BA: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==28448==  Block was alloc'd at
==28448==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==28448==    by 0x54F6868: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54F6E9F: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x54FC24D: ??? (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F93E: TTF_OpenFontIndexDPIRW (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x123C6E: Game::loadMedia() (game.cpp:30)
==28448==    by 0x124774: Game::run() (game.cpp:200)
==28448==    by 0x1276E9: Thread::main() (thread.h:43)
==28448==    by 0x133907: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==28448==    by 0x13385A: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==28448==    by 0x1337BA: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==28448==    by 0x13376F: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==28448== 
==28448== Invalid read of size 4
==28448==    at 0x54FBF50: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F76E: TTF_CloseFont (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x128E41: std::unique_ptr<_TTF_Font, void (*)(_TTF_Font*)>::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x126BC3: Game::~Game() (game.cpp:503)
==28448==    by 0x126D35: Game::~Game() (game.cpp:503)
==28448==    by 0x11884F: std::default_delete<Game>::operator()(Game*) const (unique_ptr.h:85)
==28448==    by 0x117835: std::unique_ptr<Game, std::default_delete<Game> >::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x116CC4: Client::~Client() (client.cpp:85)
==28448==    by 0x133D31: main (main.cpp:14)
==28448==  Address 0x88 is not stack'd, malloc'd or (recently) free'd
==28448== 
==28448== 
==28448== Process terminating with default action of signal 11 (SIGSEGV)
==28448==  Access not within mapped region at address 0x88
==28448==    at 0x54FBF50: FT_Done_Face (in /usr/lib/x86_64-linux-gnu/libfreetype.so.6.18.1)
==28448==    by 0x4A3F76E: TTF_CloseFont (in /usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so.0.18.0)
==28448==    by 0x128E41: std::unique_ptr<_TTF_Font, void (*)(_TTF_Font*)>::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x126BC3: Game::~Game() (game.cpp:503)
==28448==    by 0x126D35: Game::~Game() (game.cpp:503)
==28448==    by 0x11884F: std::default_delete<Game>::operator()(Game*) const (unique_ptr.h:85)
==28448==    by 0x117835: std::unique_ptr<Game, std::default_delete<Game> >::~unique_ptr() (unique_ptr.h:361)
==28448==    by 0x116CC4: Client::~Client() (client.cpp:85)
==28448==    by 0x133D31: main (main.cpp:14)
==28448==  If you believe this happened as a result of a stack
==28448==  overflow in your program's main thread (unlikely but
==28448==  possible), you can try to increase the size of the
==28448==  main thread stack using the --main-stacksize= flag.
==28448==  The main thread stack size used in this run was 8388608.
==28448== 

*/

void Client::requestId() {
    unsigned int id_input;
    std::cout << "Ingrese el ID de cliente: ";
    std::cin >> id_input;

    if (id_input > 255) {
        std::cerr << "ID inválido. Debe estar en el rango [0, 255]." << std::endl;
        throw std::invalid_argument("ID fuera de rango");
    }

    client_id = static_cast<uint8_t>(id_input);
    commandQueue->push(NEW_CLIENT); 
}

void Client::run() {
    checkIfClose();
}

void Client::stop(){
    if (!_keep_running) return;
    _keep_running.store(false); 
    gameThread->stop();
    std::cout << "Game stopped" << std::endl;
    sendThread->stop();
    std::cout << "sender stopped" << std::endl;
    recvThread->stop();
    std::cout << "receiver stopped" << std::endl;

    gameThread->join();
    sendThread->join();
    recvThread->join();
    std::cout << "Client terminado" << std::endl;
}

Client::~Client() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error al cerrar el juego: " << e.what() << std::endl;
    }
}