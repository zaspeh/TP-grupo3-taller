#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

// Client side
#define CLIENT_USAGE_COMMAND_ERROR "Error of usage: ./client <ip> <port>"
#define SPRITE_PROJECTILE_WIDTH 16
#define SPRITE_PROJECTILE_HEIGHT 16

// Server side
//#define MOVEMENT 0x15
//#define NO_MOVEMENT 0x16
#define SLEEP_DURATION_MS = 0.03333
#define MAX_CLIENTS_PER_QUEUE 100

#define BOX_HEALTH 4    
#define WIDTH_BOX 32
#define HEIGHT_BOX 32
#define PROJECTILE_RADIUS 5

// Protocol side
#define ERROR_READING_STRING "Error reading string from protocol"
#define ERROR_READING_INT "Error reading int from protocol"
#define ERROR_READING_FLOAT "Error reading float from protocol"
constexpr const char* ERROR_PREFIX = "Error: ";

// Shared
#define MOVE_LEFT 0x01
#define MOVE_RIGHT 0x02
#define JUMP 0x03
#define TAKE_WEAPON 0x04
#define SHOOT 0x05
#define LOOK_UP 0x06
#define FLOOR 0x07
#define START_MATCH 0x99
#define INFINIT_AMMO 0x11
#define PICK_ANY_WEAPON 0x12
#define NEW_CLIENT 0x77

#define NULL_WEAPON 0x0
#define GRENADE_WEAPON 20
#define BANANA_WEAPON 21
#define PEWPEWLASER_WEAPON 22
#define LASERRIFLE_WEAPON 23
#define AK_47_WEAPON 24
#define DARTGUN_WEAPON 25
#define COWBOY_WEAPON 26
#define MAGNUM_WEAPON 27
#define SHOTGUN_WEAPON 28
#define SNIPER_WEAPON 29

#define NULL_ARMOR 0x0
#define HELMET_ARMOR 30
#define CHESTPLATE_ARMOR 31
#define RESTART_MATCH 14

#define ASK_ID 12
#define LEAVE_MATCH 13

#define YELLOW_DUCK 40
#define GREY_DUCK 41
#define ORANGE_DUCK 42 
#define WHITE_DUCK 43   


constexpr const uint8_t GRASS_PLATFORM = 1;
constexpr const uint8_t DIRT_PLATFORM = 2;

constexpr const uint8_t WIDTH_DUCK = 32;
constexpr const uint8_t HEIGHT_DUCK = 32;
constexpr const uint8_t WIDTH_PLATFORM = 32;
constexpr const uint8_t HEIGHT_PLATFORM = 32;

#endif
