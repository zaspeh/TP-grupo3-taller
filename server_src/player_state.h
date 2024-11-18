#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "weapon.h"
#include "equipment.h"
#include "../common_src/game_state.h"

#include <iostream>



class PlayerState {
private:
    weapon_t nullWeapon = {{0, 0}, NULL_WEAPON, 0};
    armor_t nullArmor = {{0, 0}, NULL_ARMOR};
    duck_t duck;
    Weapon* weapon;
    Armor armor;
    Helmet helmet;

    bool isOnGround = false;
    float verticalVelocity;  // Velocidad vertical para el salto
    //const float gravity = -9.8;  // Valor de gravedad (ejemplo)
    //const float jumpStrength = 15.0;  // Fuerza del salto
    const float gravity = 3000.0f;  // Gravedad positiva (hacia abajo)
    const float jumpStrength = -800.0f;  // Fuerza del salto negativa (hacia arriba)
    const float groundLevel = 100.0f;  // Ejemplo de nivel del suelo

public:
    // Constructor por defecto
    PlayerState(uint8_t clientID, int x, int y) : 
        weapon(nullptr), 
        verticalVelocity(0.0)
    {
        duck.pos = {x, y};
        duck.id = clientID;
        duck.faceLeft = false;
        duck.isJumping = false;
        duck.isDucking = false;
        duck.isFalling = false;  // Aseguramos que inicie en false
        duck.isFlaping = false;  // Aseguramos que inicie en false
        duck.health = 1;
        duck.isAlive = true;
        duck.score = 0;
        duck.color = 0;
        duck.equipped_weapon = nullWeapon;
        duck.helmet = nullArmor;
        duck.chestplate = nullArmor;
    }

    duck_t getState() {
        return duck;
    }
    void resetPlayer(duck_t newDuck, int x , int y) {
        duck = newDuck;
        duck.pos = {x, y};
        duck.isAlive = true;
        duck.isJumping = false;
        duck.isFalling = false;   
        duck.isFlaping = false;
        duck.equipped_weapon.type = NULL_WEAPON;
        duck.chestplate.type = NULL_ARMOR;
        duck.helmet.type = NULL_ARMOR;
    }

    // Getters y Setters para cada atributo
    position_t getPosition() const { return duck.pos; }
    void setPosition(const position_t& newPosition)  { duck.pos = newPosition; }

    Weapon* getWeapon() const { return weapon; }
    uint8_t getWeaponType() const { return weapon->getId(); } // ahora en weapon
    weapon_t pickWeapon(Weapon* newWeapon);

    uint8_t getAmmo() const { return weapon->getAmmo(); } // ahora en weapon
    void setAmmo(uint8_t newAmmo) { weapon->setAmmo(newAmmo); }

    bool isAlive()  { return duck.isAlive; }
    void setAlive() { duck.isAlive = !duck.isAlive; }

    bool isFalling() const { return duck.isFalling; }
    void setFalling() { duck.isFalling = true; }

    bool isCrouched() const { return duck.isDucking; } // por què isDucking?
    void setCrouched(bool isCrouched) { duck.isDucking = isCrouched; }

    bool hasArmorEquipped() const { return armor.isEquipped(); }
    void setArmorEquipped(armor_t armr) { 
        duck.chestplate = armr;
        armor.unequip();
        if (duck.chestplate.type != NULL_ARMOR)
            armor.equip(); 
        }    

    bool hasHelmetEquipped() const { return helmet.isEquipped(); }
    void setHelmetEquipped(armor_t hmt) { 
        duck.helmet = hmt;
        helmet.unequip();
        if (duck.helmet.type != NULL_ARMOR)
            helmet.equip(); 
        }

    uint8_t getFacingDirection() const { return duck.faceLeft; }
    void setFacingDirection(uint8_t direction) { duck.faceLeft = direction; }; 
    void move(int dx, int dy, platform_t* plat, uint8_t numPlats);

    bool doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x);

    bool doNotCollideY(platform_t* plat, uint8_t numPlats, int new_y);

    /* int getScore() const;
    void setScore(int newScore);

    uint8_t getDuckColor() const;
    void setDuckColor(uint8_t color); */

    void takeDamage(uint8_t damage);

    bool shoot();

    //void crouch() { duck.isCrouched = true; }

    weapon_t dropWeapon();
    void equipArmor() { armor.equip(); }
    void equipHelmet() { helmet.equip(); }


    void updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms);

    bool checkPlatformBelow(platform_t* platforms, uint8_t numPlatforms, float newY);

    void forcePlatformState();

    void jump();
};

#endif // PLAYERSTATE_H



/*
==17795== Thread 3:
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x121991: GameState::updateProjectilsPhysics(float) (game_state.cpp:169)
==17795==    by 0x12226B: GameState::updatePlayers(float) (game_state.cpp:248)
==17795==    by 0x11BAC0: GameLoop::run() (gameloop.cpp:46)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Thread 4:
==17795== Syscall param socketcall.sendto(msg) points to uninitialised byte(s)
==17795==    at 0x4CCA8FE: __libc_send (send.c:28)
==17795==    by 0x4CCA8FE: send (send.c:23)
==17795==    by 0x11302D: Socket::sendsome(void const*, unsigned int, bool*) (socket.cpp:279)
==17795==    by 0x11322D: Socket::sendall(void const*, unsigned int, bool*) (socket.cpp:352)
==17795==    by 0x10E2B2: Protocol::sendUint8(unsigned char, bool&) (protocol.cpp:28)
==17795==    by 0x110859: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:119)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==  Address 0x69c9d04 is on thread 4's stack
==17795==  in frame #3, created by Protocol::sendUint8(unsigned char, bool&) (protocol.cpp:27)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x1103E0: ServerProtocol::sendDucks(duck_t*, unsigned char, bool&) (serverprotocol.cpp:69)
==17795==    by 0x11087B: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:120)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Syscall param socketcall.sendto(msg) points to uninitialised byte(s)
==17795==    at 0x4CCA8FE: __libc_send (send.c:28)
==17795==    by 0x4CCA8FE: send (send.c:23)
==17795==    by 0x11302D: Socket::sendsome(void const*, unsigned int, bool*) (socket.cpp:279)
==17795==    by 0x11322D: Socket::sendall(void const*, unsigned int, bool*) (socket.cpp:352)
==17795==    by 0x10E686: Protocol::sendInt(int, bool&) (protocol.cpp:64)
==17795==    by 0x1101BA: ServerProtocol::sendPosition(position_t, bool&) (serverprotocol.cpp:47)
==17795==    by 0x110440: ServerProtocol::sendPlatforms(platform_t*, unsigned char, bool&) (serverprotocol.cpp:76)
==17795==    by 0x1108C4: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:122)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==  Address 0x69c9c94 is on thread 4's stack
==17795==  in frame #3, created by Protocol::sendInt(int, bool&) (protocol.cpp:62)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x110747: ServerProtocol::sendProjectiles(projectile_t*, unsigned char, bool&) (serverprotocol.cpp:99)
==17795==    by 0x11099F: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:128)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x1107B6: ServerProtocol::sendDroppedWeapons(weapon_t*, unsigned char, bool) (serverprotocol.cpp:107)
==17795==    by 0x1109EE: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:130)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x11081F: ServerProtocol::sendDroppedArmors(armor_t*, unsigned char, bool) (serverprotocol.cpp:113)
==17795==    by 0x110A3D: ServerProtocol::sendLevel(level_t&, bool&) (serverprotocol.cpp:132)
==17795==    by 0x11008A: ServerProtocol::sendGameState(game_state_t&, bool&) (serverprotocol.cpp:13)
==17795==    by 0x13B3C6: Sender::run() (sender.cpp:35)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 


------------------------------

==17795== Thread 3:
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x1380FA: PlayerState::updatePosition(float, platform_t*, unsigned char) (player_state.cpp:45)
==17795==    by 0x1221BB: GameState::updatePlayers(float) (game_state.cpp:240)
==17795==    by 0x11BAC0: GameLoop::run() (gameloop.cpp:46)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x138127: PlayerState::updatePosition(float, platform_t*, unsigned char) (player_state.cpp:45)
==17795==    by 0x1221BB: GameState::updatePlayers(float) (game_state.cpp:240)
==17795==    by 0x11BAC0: GameLoop::run() (gameloop.cpp:46)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 

==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x137EE0: PlayerState::doNotCollideX(platform_t*, unsigned char, int) (player_state.cpp:6)
==17795==    by 0x137FC9: PlayerState::move(int, int, platform_t*, unsigned char) (player_state.cpp:23)
==17795==    by 0x1210F8: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:71)
==17795==    by 0x11BC75: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==17795==    by 0x139FC1: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==17795==    by 0x13A765: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==17795==    by 0x13A659: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==17795==    by 0x13A50A: std::_Function_handler<void (), Receiver::run()::{lambda()#1}>::_M_invoke(std::_Any_data const&) (std_function.h:290)
==17795==    by 0x11CD1B: std::function<void ()>::operator()() const (std_function.h:590)
==17795==    by 0x11B8F7: GameLoop::ejecutar_comandos() (gameloop.cpp:25)
==17795==    by 0x11BA4F: GameLoop::run() (gameloop.cpp:41)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x137F12: PlayerState::doNotCollideX(platform_t*, unsigned char, int) (player_state.cpp:6)
==17795==    by 0x137FC9: PlayerState::move(int, int, platform_t*, unsigned char) (player_state.cpp:23)
==17795==    by 0x1210F8: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:71)
==17795==    by 0x11BC75: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==17795==    by 0x139FC1: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==17795==    by 0x13A765: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==17795==    by 0x13A659: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==17795==    by 0x13A50A: std::_Function_handler<void (), Receiver::run()::{lambda()#1}>::_M_invoke(std::_Any_data const&) (std_function.h:290)
==17795==    by 0x11CD1B: std::function<void ()>::operator()() const (std_function.h:590)
==17795==    by 0x11B8F7: GameLoop::ejecutar_comandos() (gameloop.cpp:25)
==17795==    by 0x11BA4F: GameLoop::run() (gameloop.cpp:41)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 

==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x137EE0: PlayerState::doNotCollideX(platform_t*, unsigned char, int) (player_state.cpp:6)
==17795==    by 0x137FC9: PlayerState::move(int, int, platform_t*, unsigned char) (player_state.cpp:23)
==17795==    by 0x121098: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:67)
==17795==    by 0x11BC75: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==17795==    by 0x139FC1: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==17795==    by 0x13A765: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==17795==    by 0x13A659: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==17795==    by 0x13A50A: std::_Function_handler<void (), Receiver::run()::{lambda()#1}>::_M_invoke(std::_Any_data const&) (std_function.h:290)
==17795==    by 0x11CD1B: std::function<void ()>::operator()() const (std_function.h:590)
==17795==    by 0x11B8F7: GameLoop::ejecutar_comandos() (gameloop.cpp:25)
==17795==    by 0x11BA4F: GameLoop::run() (gameloop.cpp:41)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x137F12: PlayerState::doNotCollideX(platform_t*, unsigned char, int) (player_state.cpp:6)
==17795==    by 0x137FC9: PlayerState::move(int, int, platform_t*, unsigned char) (player_state.cpp:23)
==17795==    by 0x121098: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:67)
==17795==    by 0x11BC75: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==17795==    by 0x139FC1: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==17795==    by 0x13A765: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==17795==    by 0x13A659: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==17795==    by 0x13A50A: std::_Function_handler<void (), Receiver::run()::{lambda()#1}>::_M_invoke(std::_Any_data const&) (std_function.h:290)
==17795==    by 0x11CD1B: std::function<void ()>::operator()() const (std_function.h:590)
==17795==    by 0x11B8F7: GameLoop::ejecutar_comandos() (gameloop.cpp:25)
==17795==    by 0x11BA4F: GameLoop::run() (gameloop.cpp:41)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 



-------------

==17795== Thread 5:
==17795== Conditional jump or move depends on uninitialised value(s)
==17795==    at 0x120AF0: GameState::removePlayer(unsigned char) (game_state.cpp:27)
==17795==    by 0x11BD09: GameLoop::removePlayer(unsigned char) (gameloop.cpp:75)
==17795==    by 0x13A20D: Receiver::run() (receiver.cpp:38)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795==  Uninitialised value was created by a heap allocation
==17795==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17795==    by 0x11B7FF: GameLoop::initGame() (gameloop.cpp:13)
==17795==    by 0x113671: Accepter::run() (accepter.cpp:14)
==17795==    by 0x11466B: Thread::main() (thread.h:43)
==17795==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==17795==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==17795==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==17795==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==17795==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==17795==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==17795==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==17795==    by 0x4CC8A03: clone (clone.S:100)
==17795== 


*/