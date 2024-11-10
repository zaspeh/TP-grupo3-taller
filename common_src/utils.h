#ifndef UTILS_H
#define UTILS_H

// Client side
#define CLIENT_USAGE_COMMAND_ERROR "Error of usage: ./client <ip> <port>"

// Server side
//#define MOVEMENT 0x15
//#define NO_MOVEMENT 0x16
#define SLEEP_DURATION_MS = 0.03333
#define MAX_CLIENTS_PER_QUEUE 100

#define NULL_WEAPON 0x0
#define GRENADE_WEAPON 0x1
#define BANANA_WEAPON 0x2
#define PEWPEWLASER_WEAPON 0x3
#define LASERRIFLE_WEAPON 0x4
#define AK_47_WEAPON 0x5
#define DARTGUN_WEAPON 0x6
#define CHAINSAW_WEAPON 0x7

#define NULL_ARMOR 0x0
#define HELMET_ARMOR 0x1
#define CHESTPLATE_ARMOR 0x2

// Protocol side
#define ERROR_READING_STRING "Error reading string from protocol"
#define ERROR_READING_INT "Error reading int from protocol"
#define ERROR_READING_FLOAT "Error reading float from protocol"
#define EXCEPTION "Error: "

// Shared
#define MOVE_LEFT 0x01
#define MOVE_RIGHT 0x02
#define JUMP 0x03
#define TAKE_WEAPON 0x04
#define SHOOT 0x05
#define LOOK_UP 0x06
#define FLOOR 0x07
#define START_MATCH 0x99

#define NEW_CLIENT 0x77




constexpr const uint8_t GRASS_PLATFORM = 1;
constexpr const uint8_t DIRT_PLATFORM = 2;

constexpr const uint8_t WIDTH_DUCK = 32;
constexpr const uint8_t HEIGHT_DUCK = 32;
constexpr const uint8_t WIDTH_PLATFORM = 32;
constexpr const uint8_t HEIGHT_PLATFORM = 32;

#endif
