#include "armor.h"
#include <iostream>

Armor::Armor(armor_t armorState, SDL_Renderer* renderer) {
    updateState(armorState);
    try {
        armors.emplace(CHESTPLATE_ARMOR, std::make_unique<LTexture>(renderer));
        armors.emplace(HELMET_ARMOR, std::make_unique<LTexture>(renderer));
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create armor textures: " << e.what() << std::endl;
    }
}

bool Armor::loadTexture() {
    bool success = true;

    if (chestplateState.type == CHESTPLATE_ARMOR) 
        success &= armors[CHESTPLATE_ARMOR]->loadFromFile("client_src/armors/chestplate.png");

    if (helmetState.type == HELMET_ARMOR) 
        success &= armors[HELMET_ARMOR]->loadFromFile("client_src/armors/helmet.png");

    if (!success) {
        std::cerr << "Failed to load one or more armor textures." << std::endl;
    }

    return success;
}

void Armor::render(int x, int y, bool faceLeft, uint8_t type, const Camera& camera, float zoom) {
    if (type == CHESTPLATE_ARMOR)
        renderArmorPiece(CHESTPLATE_ARMOR, chestplateState, x, y, faceLeft, camera, zoom);
    if (type == HELMET_ARMOR)
        renderArmorPiece(HELMET_ARMOR, helmetState, x, y, faceLeft, camera, zoom);
}

void Armor::renderArmorPiece(uint8_t armorType, armor_t& armorState, int x, int y, bool faceLeft, const Camera& camera, float zoom) {
    auto it = armors.find(armorType);
    SDL_Rect scaleRect = {x, y, 0, 0};
    if (it != armors.end() && armorState.type != NULL_ARMOR) {
        LTexture* texture = it->second.get();
        scaleRect.w = texture->getWidth() * 2.15;
        scaleRect.h = texture->getHeight() * 2.15;

        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        // Desplazamientos específicos para cada pieza de armadura
        float xOffset = 0;
        float yOffset = 0;

        if (armorType == CHESTPLATE_ARMOR) {
            xOffset = 25;  // Ajuste horizontal para el chestplate
            yOffset = 28;  // Ajuste vertical para el chestplate
        } else if (armorType == HELMET_ARMOR) {
            xOffset = 7;   // Ajuste horizontal para el helmet
            yOffset = 0; // Ajuste vertical para el helmet
        }

        //SDL_Rect destRect = scaleRect;

        SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
        SDL_Rect destRect = {
            screenPos.x,
            screenPos.y,
            static_cast<int>(scaleRect.w * zoom),
            static_cast<int>(scaleRect.h * zoom)
        };

        // Aplica los desplazamientos
        texture->render(destRect.x + xOffset * (faceLeft ? -1 : 1), destRect.y + yOffset, nullptr, &destRect, flip);
    } else {
        std::cerr << "Texture not found or invalid for armor type: " << static_cast<int>(armorType) << std::endl;
    }
}

int Armor::getType() {
    if (chestplateState.type == CHESTPLATE_ARMOR) {
        return CHESTPLATE_ARMOR;
    } else if (helmetState.type == HELMET_ARMOR) {
        return HELMET_ARMOR;
    }
    return NULL_ARMOR;
}

void Armor::updateState(armor_t arm) {
    chestplateState = nullArmor;
    helmetState = nullArmor;
    if (arm.type == CHESTPLATE_ARMOR) 
        chestplateState = arm;

    if (arm.type == HELMET_ARMOR)
        helmetState = arm;

}

armor_t Armor::getState() {
    
    if (chestplateState.type == CHESTPLATE_ARMOR)
        return chestplateState;
    else if (helmetState.type == HELMET_ARMOR)
        return helmetState;
    else 
        return nullArmor;
}


/* 

zaspeh@44931392:~/Escritorio/tp/TP-grupo3-taller$ valgrind ./client localhost 8080
==22439== Memcheck, a memory error detector
==22439== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==22439== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==22439== Command: ./client localhost 8080
==22439== 
Ingrese el ID de cliente: 0
ID inválido. Debe estar en el rango [0, 255].
ID inválido. Debe estar en el rango [0, 255].
Id inicializando en: 0
REceiver: 0
Sender: 0
Comando: 119
Client ID: 0
Ingrese 'q' para cerrar el juego: ==22439== Thread 4:
==22439== Invalid read of size 8
==22439==    at 0x40286A8: strncmp (strcmp.S:172)
==22439==    by 0x400668D: is_dst (dl-load.c:216)
==22439==    by 0x400810E: _dl_dst_count (dl-load.c:253)
==22439==    by 0x400810E: expand_dynamic_string_token (dl-load.c:395)
==22439==    by 0x40082B7: fillin_rpath.isra.0 (dl-load.c:483)
==22439==    by 0x4008602: decompose_rpath (dl-load.c:654)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:696)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:677)
==22439==    by 0x400ABF5: _dl_map_object (dl-load.c:2165)
==22439==    by 0x4003494: openaux (dl-deps.c:64)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x4003C7B: _dl_map_object_deps (dl-deps.c:248)
==22439==    by 0x400EA0E: dl_open_worker_begin (dl-open.c:592)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400DF99: dl_open_worker (dl-open.c:782)
==22439==  Address 0x83fa4e9 is 9 bytes inside a block of size 15 alloc'd
==22439==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x40271DF: malloc (rtld-malloc.h:56)
==22439==    by 0x40271DF: strdup (strdup.c:42)
==22439==    by 0x4008594: decompose_rpath (dl-load.c:629)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:696)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:677)
==22439==    by 0x400ABF5: _dl_map_object (dl-load.c:2165)
==22439==    by 0x4003494: openaux (dl-deps.c:64)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x4003C7B: _dl_map_object_deps (dl-deps.c:248)
==22439==    by 0x400EA0E: dl_open_worker_begin (dl-open.c:592)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400DF99: dl_open_worker (dl-open.c:782)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400E34D: _dl_open (dl-open.c:883)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x40286A8: strncmp (strcmp.S:172)
==22439==    by 0x400668D: is_dst (dl-load.c:216)
==22439==    by 0x4007F79: _dl_dst_substitute (dl-load.c:295)
==22439==    by 0x40082B7: fillin_rpath.isra.0 (dl-load.c:483)
==22439==    by 0x4008602: decompose_rpath (dl-load.c:654)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:696)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:677)
==22439==    by 0x400ABF5: _dl_map_object (dl-load.c:2165)
==22439==    by 0x4003494: openaux (dl-deps.c:64)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x4003C7B: _dl_map_object_deps (dl-deps.c:248)
==22439==    by 0x400EA0E: dl_open_worker_begin (dl-open.c:592)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400DF99: dl_open_worker (dl-open.c:782)
==22439==  Address 0x83fa4e9 is 9 bytes inside a block of size 15 alloc'd
==22439==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x40271DF: malloc (rtld-malloc.h:56)
==22439==    by 0x40271DF: strdup (strdup.c:42)
==22439==    by 0x4008594: decompose_rpath (dl-load.c:629)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:696)
==22439==    by 0x400ABF5: cache_rpath (dl-load.c:677)
==22439==    by 0x400ABF5: _dl_map_object (dl-load.c:2165)
==22439==    by 0x4003494: openaux (dl-deps.c:64)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x4003C7B: _dl_map_object_deps (dl-deps.c:248)
==22439==    by 0x400EA0E: dl_open_worker_begin (dl-open.c:592)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400DF99: dl_open_worker (dl-open.c:782)
==22439==    by 0x4DF5A97: _dl_catch_exception (dl-error-skeleton.c:208)
==22439==    by 0x400E34D: _dl_open (dl-open.c:883)
==22439== 
Initialized ducks vector with 0 ducks.
Initialized ducks vector with 81 platforms.
Tamaño de spawns: 10
Creando nuevo spawn: 0
Creando nuevo spawn: 1
Creando nuevo spawn: 0
Creando nuevo spawn: 1
Creando nuevo spawn: 9
Creando nuevo spawn: 0
Creando nuevo spawn: 0
Creando nuevo spawn: 1
Creando nuevo spawn: 0
Creando nuevo spawn: 1
Creando nuevo spawn: 7
Creando nuevo spawn: 0
Cambiando el tamaño de las cajas
Cajas reziseadas.
Cargando boxes
Cargando boxes
Cargando boxes
Cargando boxes
Cargando boxes
Cargando boxes
Creando nuevo pato 
Error: Invalid weapon type 0
New duck initialized with ID 
q
q
Error: socket recv failedBad file descriptor
==22439== Invalid read of size 8
==22439==    at 0x55B1993: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B1F: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c7dbc0 is 20,928 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x55B19F3: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B1F: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c78a70 is 112 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x55B19FC: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B1F: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c79b60 is 4,448 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid write of size 4
==22439==    at 0x55B182B: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B1F: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c78a70 is 112 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x55B1B5D: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==  Address 0x5c7dbb0 is 20,912 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid write of size 8
==22439==    at 0x55B1B67: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==  Address 0x5c7dbb8 is 20,920 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D13E03: pthread_cond_broadcast@@GLIBC_2.3.2 (pthread_cond_broadcast.c:42)
==22439==    by 0x55B1B72: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c79b54 is 4,436 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x55B0C44: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B7A: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c79ac0 is 4,288 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x55B0C68: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B7A: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==  Address 0x5c79ac8 is 4,296 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D14233: pthread_cond_signal@@GLIBC_2.3.2 (pthread_cond_signal.c:41)
==22439==    by 0x55B0C90: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1B7A: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B1D76: ??? (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B2DBF: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==  Address 0x5c78a64 is 100 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D1AA74: __pthread_mutex_unlock_usercnt (pthread_mutex_unlock.c:51)
==22439==    by 0x4D1AA74: pthread_mutex_unlock@@GLIBC_2.2.5 (pthread_mutex_unlock.c:368)
==22439==    by 0x55B2DCA: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==  Address 0x5c78a28 is 40 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D1AA8C: __pthread_mutex_unlock_usercnt (pthread_mutex_unlock.c:65)
==22439==    by 0x4D1AA8C: pthread_mutex_unlock@@GLIBC_2.2.5 (pthread_mutex_unlock.c:368)
==22439==    by 0x55B2DCA: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==  Address 0x5c78a24 is 36 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid write of size 4
==22439==    at 0x4D1AA90: __pthread_mutex_unlock_usercnt (pthread_mutex_unlock.c:62)
==22439==    by 0x4D1AA90: pthread_mutex_unlock@@GLIBC_2.2.5 (pthread_mutex_unlock.c:368)
==22439==    by 0x55B2DCA: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==  Address 0x5c78a20 is 32 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D1AA97: lll_mutex_unlock_optimized (pthread_mutex_unlock.c:39)
==22439==    by 0x4D1AA97: __pthread_mutex_unlock_usercnt (pthread_mutex_unlock.c:68)
==22439==    by 0x4D1AA97: pthread_mutex_unlock@@GLIBC_2.2.5 (pthread_mutex_unlock.c:368)
==22439==    by 0x55B2DCA: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==  Address 0x5c78a28 is 40 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 4
==22439==    at 0x4D1AAA4: lll_mutex_unlock_optimized (pthread_mutex_unlock.c:43)
==22439==    by 0x4D1AAA4: __pthread_mutex_unlock_usercnt (pthread_mutex_unlock.c:68)
==22439==    by 0x4D1AAA4: pthread_mutex_unlock@@GLIBC_2.2.5 (pthread_mutex_unlock.c:368)
==22439==    by 0x55B2DCA: xcb_flush (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x85FD355: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==  Address 0x5c78a18 is 24 bytes inside a block of size 21,168 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8B8: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x55B4204: xcb_connect_to_fd (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x55B4E6B: xcb_connect_to_display_with_auth_info (in /usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0)
==22439==    by 0x5128EE9: _XConnectXCB (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x5118B68: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x85FD38E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85EEF94: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DE71E: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x495EFD6: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x124365: Game::render() (game.cpp:228)
==22439==    by 0x123B20: Game::run() (game.cpp:176)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==    by 0x131425: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22439==  Address 0x80e7688 is 392 bytes inside a block of size 424 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x85E0F21: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85E106C: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x510A891: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x85EF637: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85E1330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x85DCD77: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22439==    by 0x4967B2C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x4967F0C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493EE3B: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493D035: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DB60: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x493F99F: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x123B38: Game::run() (game.cpp:178)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==    by 0x131425: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22439==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22439==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22439==    by 0x4DA6A03: clone (clone.S:100)
==22439==  Address 0x152e4bf0 is 0 bytes inside a block of size 240 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x4945559: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x49456F0: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x493D0C1: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12531D: Game::init() (game.cpp:482)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==    by 0x131425: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22439==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22439==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x51088BB: XCheckIfEvent (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x4962B5C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BE601: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BEA9E: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12385D: Game::processEvents() (game.cpp:99)
==22439==    by 0x123A63: Game::run() (game.cpp:162)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==    by 0x131425: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22439==  Address 0x5c75c68 is 2,408 bytes inside a block of size 4,704 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8C0: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x5118B31: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x5108978: XCheckIfEvent (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x4962B5C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BE601: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BEA9E: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12385D: Game::processEvents() (game.cpp:99)
==22439==    by 0x123A63: Game::run() (game.cpp:162)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==    by 0x131425: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22439==  Address 0x5c75378 is 120 bytes inside a block of size 4,704 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8C0: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x5118B31: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x512DBFB: _XEventsQueued (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x510895C: XCheckIfEvent (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x4962B5C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BE601: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BEA9E: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12385D: Game::processEvents() (game.cpp:99)
==22439==    by 0x123A63: Game::run() (game.cpp:162)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==  Address 0x5c753f8 is 248 bytes inside a block of size 4,704 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8C0: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x5118B31: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439== 
==22439== Invalid read of size 8
==22439==    at 0x512DC06: _XEventsQueued (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x510895C: XCheckIfEvent (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x4962B5C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BE601: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BEA9E: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12385D: Game::processEvents() (game.cpp:99)
==22439==    by 0x123A63: Game::run() (game.cpp:162)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==  Address 0x5c75d30 is 2,608 bytes inside a block of size 4,704 free'd
==22439==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x510A8C0: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1254F7: Game::stop() (game.cpp:515)
==22439==    by 0x116916: Client::stop() (client.cpp:91)
==22439==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22439==    by 0x116849: Client::run() (client.cpp:79)
==22439==    by 0x1319F3: main (main.cpp:11)
==22439==  Block was alloc'd at
==22439==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22439==    by 0x5118B31: XOpenDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x496AE7D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x493DA48: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x489D426: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x1252BE: Game::init() (game.cpp:477)
==22439==    by 0x123A07: Game::run() (game.cpp:151)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439== 
==22439== 
==22439== Process terminating with default action of signal 11 (SIGSEGV)
==22439==  Access not within mapped region at address 0x48
==22439==    at 0x512DC10: _XEventsQueued (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x510895C: XCheckIfEvent (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22439==    by 0x4962B5C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BE601: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x48BEA9E: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22439==    by 0x12385D: Game::processEvents() (game.cpp:99)
==22439==    by 0x123A63: Game::run() (game.cpp:162)
==22439==    by 0x125B29: Thread::main() (thread.h:43)
==22439==    by 0x1315DD: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22439==    by 0x131530: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22439==    by 0x131490: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22439==    by 0x131445: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22439==  If you believe this happened as a result of a stack
==22439==  overflow in your program's main thread (unlikely but
==22439==  possible), you can try to increase the size of the
==22439==  main thread stack using the --main-stacksize= flag.
==22439==  The main thread stack size used in this run was 8388608.
==22439== 
==22439== HEAP SUMMARY:
==22439==     in use at exit: 9,974,509 bytes in 94,774 blocks
==22439==   total heap usage: 621,521 allocs, 526,747 frees, 143,298,764 bytes allocated
==22439== 
==22439== LEAK SUMMARY:
==22439==    definitely lost: 612,368 bytes in 38,273 blocks
==22439==    indirectly lost: 192 bytes in 3 blocks
==22439==      possibly lost: 7,379,246 bytes in 50,180 blocks
==22439==    still reachable: 1,982,703 bytes in 6,318 blocks
==22439==         suppressed: 0 bytes in 0 blocks
==22439== Rerun with --leak-check=full to see details of leaked memory
==22439== 
==22439== For lists of detected and suppressed errors, rerun with: -s
==22439== ERROR SUMMARY: 28 errors from 23 contexts (suppressed: 1 from 1)
Violación de segmento (`core' generado)

 */