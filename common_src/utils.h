#ifndef UTILS_H
#define UTILS_H

// Client side
#define CLIENT_USAGE_COMMAND_ERROR "Error of usage: ./client <ip> <port>"

// Server side
//#define MOVEMENT 0x15
//#define NO_MOVEMENT 0x16
#define SLEEP_DURATION_MS = 1/30;

#define MAX_CLIENTS_PER_QUEUE 100

// Protocol side
#define ERROR_READING_STRING "Error reading string from protocol"
#define ERROR_READING_INT "Error reading int from protocol"
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

#endif
