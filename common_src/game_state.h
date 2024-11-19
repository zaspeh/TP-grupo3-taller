#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

#define MAX_DUCKS 10
#define MAX_PROJECTILES 3000
#define MAX_ITEMS 500
#define MAX_LEVELS 5
#define MAX_PLATFORMS 1000
#define MAX_SPAWN_PLACES 20
#define MAX_BOXES 20


typedef struct {
    int x;
    int y;
} position_t;

// Representa plataformas en el escenario
typedef struct {
    position_t pos; 
    uint8_t type;   
} platform_t;

// Representa un arma en el juego
typedef struct {
    position_t pos;
    uint8_t type;   
    uint8_t ammo;    
} weapon_t;

// Representa una armadura o casco en el juego
typedef struct {
    position_t pos;
    uint8_t type;    
} armor_t;



// Representa puntos de aparición donde aparecen armas y armaduras
typedef struct {
    position_t pos;    
    bool is_active;    
    weapon_t weapon;   
    armor_t armor;     
} spawn_place_t;

// Representa cajas en el escenario que pueden contener items o ser explosivas
typedef struct {
    position_t pos;      
    uint8_t health;       
    bool is_explosive;   
} box_t;

// Representa proyectiles lanzados por armas en el juego
typedef struct {
    position_t pos;  
    uint8_t type;    
    bool is_active;  
} projectile_t;

// Representa el estado de un pato
typedef struct {
    position_t pos; 
    uint8_t id;                 
    bool faceLeft;         
    bool isJumping;         
    bool isDucking;         
    bool isFalling;         
    bool isFlaping;         
    uint8_t health;             
    bool isAlive;            
    uint8_t score;              
    uint8_t color;              
    weapon_t equipped_weapon;    
    armor_t helmet;   
    armor_t chestplate;
} duck_t;

// Representa un nivel completo con todos sus elementos
typedef struct {
    uint8_t num_ducks;
    duck_t ducks[MAX_DUCKS];
    uint8_t num_platforms;
    platform_t platforms[MAX_PLATFORMS];
    uint8_t num_spawn_places;
    spawn_place_t spawn_places[MAX_SPAWN_PLACES];
    uint8_t num_boxes;
    box_t boxes[MAX_BOXES];
    uint8_t num_projectiles;
    projectile_t projectiles[MAX_PROJECTILES];
    uint8_t num_dropped_weapons;
    weapon_t dropped_weapons[MAX_ITEMS];
    uint8_t num_dropped_armors;
    armor_t dropped_armors[MAX_ITEMS];
    uint8_t num_explosions;
    position_t explosions[MAX_ITEMS];
} level_t;

// Estado global del juego
typedef struct {
    level_t level;
    uint8_t current_level;
    uint8_t round;        
    uint8_t winning_score; 
} game_state_t;

#endif /* __GAME_STATE_H__ */