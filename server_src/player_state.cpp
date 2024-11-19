#include "player_state.h"

bool PlayerState::doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x) {
    for (int i = 0; i < numPlats; i++) {
        if (duck.pos.y + HEIGHT_DUCK > plat[i].pos.y && 
            duck.pos.y < plat[i].pos.y + HEIGHT_PLATFORM) {

            // Validar correctamente si estamos dentro de la plataforma en X
            if (new_x > plat[i].pos.x && 
                new_x < plat[i].pos.x + WIDTH_PLATFORM) {
                return false;
            }
        }
    }
    return true;
}

void PlayerState::move(int dx, int dy, platform_t* plat, uint8_t numPlats) {
    // Movimiento horizontal
    if (dx != 0) {
        int newX = duck.pos.x + dx;
        if (doNotCollideX(plat, numPlats, newX)) {
            duck.pos.x = newX;
        }
    }
}

void PlayerState::updateWeapon() {
    if (weapon != nullptr && weapon->getAmmo() == 0 && (weapon->getType() == GRENADE_WEAPON || weapon->getType() == BANANA_WEAPON)) {
        duck.equipped_weapon.type = NULL_WEAPON;
    }
}

void PlayerState::updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms, position_t* explotions, uint8_t numExplotions) {
    deltaTime = std::min(deltaTime, 0.016f);
    
    // actualizo las posiciones
    duck.equipped_weapon.pos = duck.pos;
    duck.helmet.pos = duck.pos;
    duck.chestplate.pos = duck.pos;

    verticalVelocity += gravity * deltaTime;
    float newY = duck.pos.y + (verticalVelocity * deltaTime);
    
    isOnGround = false;
    bool hitCeiling = false;
    
    for (int i = 0; i < numPlatforms; i++) {
        // Verificación más precisa de la intersección horizontal
        bool horizontalOverlap = (duck.pos.x + WIDTH_DUCK > platforms[i].pos.x + 20) && // Añadimos un pequeño margen
                                (duck.pos.x < platforms[i].pos.x + WIDTH_PLATFORM);  // para evitar colisiones fantasma
        
        if (horizontalOverlap) {
            // Colisión con el suelo - añadimos un margen de tolerancia
            if (newY + HEIGHT_DUCK > platforms[i].pos.y && 
                duck.pos.y + HEIGHT_DUCK <= platforms[i].pos.y + 10) { // Margen de tolerancia
                duck.pos.y = platforms[i].pos.y - HEIGHT_DUCK;
                verticalVelocity = 0;
                isOnGround = true;
                duck.isJumping = false;
                duck.isFalling = false;
                duck.isFlaping = false;
                break;
            }
            // Colisión con el techo - mejoramos la detección
            else if (newY < platforms[i].pos.y + HEIGHT_PLATFORM && 
                     duck.pos.y >= platforms[i].pos.y + HEIGHT_PLATFORM - 5) { // Reducimos el margen de colisión
                if (duck.isJumping || verticalVelocity < 0) {
                    duck.pos.y = platforms[i].pos.y + HEIGHT_PLATFORM;
                    verticalVelocity = 0;
                    hitCeiling = true;
                    duck.isJumping = false;
                    duck.isFalling = true;
                }
                break;
            }
        }
    }

    // si choca con una explosion...
    for (int i = 0; i < numExplotions; i++) {
        bool horizontalOverlap = (duck.pos.x + WIDTH_DUCK > explotions[i].x) &&
                                 (duck.pos.x < explotions[i].x + WIDTH_EXPLOTION);
        bool verticalOverlap = (duck.pos.y + HEIGHT_DUCK > explotions[i].y) &&
                                (duck.pos.y < explotions[i].y + HEIGHT_EXPLOTION);
        if (horizontalOverlap && verticalOverlap) {
            duck.isAlive = false;
        }
    }

    // Si no hay colisiones, actualizar la posición
    if (!isOnGround && !hitCeiling) {
        duck.pos.y = newY;
        duck.isFalling = verticalVelocity > 0;
    }

    float maxFallSpeed = 800.0f;
    if (duck.isFlaping) {
        maxFallSpeed = 300.0f;
    }
    if (verticalVelocity > maxFallSpeed) {
        verticalVelocity = maxFallSpeed;
    }

    if(duck.pos.y > 1024)   
        duck.isAlive = false;

    
}

/*
==14369== Thread 3:
==14369== Conditional jump or move depends on uninitialised value(s)
==14369==    at 0x1383BF: PlayerState::updatePosition(float, platform_t*, unsigned char) (player_state.cpp:45)
==14369==    by 0x1221BB: GameState::updatePlayers(float) (game_state.cpp:240)
==14369==    by 0x11BAC0: GameLoop::run() (gameloop.cpp:46)
==14369==    by 0x11466B: Thread::main() (thread.h:43)
==14369==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==14369==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==14369==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==14369==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==14369==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==14369==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==14369==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==14369==    by 0x4CC8A03: clone (clone.S:100)
==14369== 
==14369== Conditional jump or move depends on uninitialised value(s)
==14369==    at 0x1383EB: PlayerState::updatePosition(float, platform_t*, unsigned char) (player_state.cpp:45)
==14369==    by 0x1221BB: GameState::updatePlayers(float) (game_state.cpp:240)
==14369==    by 0x11BAC0: GameLoop::run() (gameloop.cpp:46)
==14369==    by 0x11466B: Thread::main() (thread.h:43)
==14369==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==14369==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==14369==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==14369==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==14369==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==14369==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==14369==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==14369==    by 0x4CC8A03: clone (clone.S:100)
==14369== 

*/

void PlayerState::jump() {
    // Solo permitir saltar si estamos en el suelo y no estamos levitando
    if (isOnGround && !duck.isFlaping) { // Solo permitir saltar si estamos en el suelo
        duck.isJumping = true;
        duck.isFalling = false;
        verticalVelocity = jumpStrength;
        isOnGround = false;  // Inmediatamente nos quitamos del suelo
    } else if (!isOnGround){
        duck.isFlaping = !duck.isFlaping;
    }



}

void PlayerState::takeDamage(uint8_t damage) {
    if (armor.isEquipped()) {
        armor.absorb_hit();
        return;
    }
    if (helmet.isEquipped()) {
        helmet.absorb_hit();
        return;
    }
    
    duck.health -= damage;
    if (duck.health <= 0) {
        setAlive();
    }
}

weapon_t PlayerState::pickWeapon(Weapon* newWeapon) {
    weapon_t weaponST = {
        {0, 0},
        NULL_WEAPON
    };
    if (weapon) {
        std::cout << "Dropping weapon\n";
        weapon_t pickedWeapon = dropWeapon();
        weaponST = pickedWeapon;
    }
    weapon = newWeapon;
    duck.equipped_weapon.type = newWeapon->getId();
    std::cout << "Tipo de arma: " << static_cast<int>(duck.equipped_weapon.type) << std::endl;
    return weaponST;
}

weapon_t PlayerState::dropWeapon() {
    weapon_t weaponST = {
        {0, 0},
        NULL_WEAPON,
        0
    };
    if (weapon != nullptr) {
        weaponST = duck.equipped_weapon;
        weaponST.ammo = weapon->getAmmo();  // Guardar la munición actual
        weapon = nullptr;
        duck.equipped_weapon.type = NULL_WEAPON;
    }
    return weaponST;
}

bool PlayerState::shoot() {
    bool returnValue = false;

    if (weapon != nullptr) {
        // Verifica si el arma es una granada
        if (weapon->getType() == GRENADE_WEAPON) {
            Grenade* grenade = dynamic_cast<Grenade*>(weapon); // Downcasting
            if (grenade != nullptr && grenade->getPinPulled()) {
                grenade->throw_grenade();
            }
        }
        // Ejecuta el disparo independientemente del tipo de arma
        returnValue = weapon->shoot(infinitAmmo); // Cambios de munición, etc.
    }
    
    return returnValue;
}