#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <stdbool.h>
#include <stdint.h>

#define MAX_DUCKS 10
#define MAX_PROJECTILES 100
#define MAX_ITEMS 50
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
    position_t pos; // Posición de la plataforma
    uint8_t type;   // Tipo de plataforma (ej.: suelo, muro, etc.)
} platform_t;

// Representa un arma en el juego
typedef struct {
    position_t pos;
    uint8_t type;    // Tipo de arma (ej.: pistola, escopeta, etc.)
} weapon_t;

// Representa una armadura o casco en el juego
typedef struct {
    position_t pos;
    uint8_t type;     // Tipo de armadura/casco
} armor_t;

// Representa puntos de aparición donde aparecen armas y armaduras
typedef struct {
    position_t pos;    // Posición del spawn place en el nivel
    bool is_active;    // Si se puede spawnear
    weapon_t weapon;   // Arma que puede aparecer en este spawn place
    armor_t armor;     // Armadura o casco que puede aparecer en este spawn place
} spawn_place_t;

// Representa cajas en el escenario que pueden contener items o ser explosivas
typedef struct {
    position_t pos;      // Posición de la caja en el nivel
    bool is_explosive;   // Si la caja es explosiva
    bool is_destroyed;   // Estado de destrucción de la caja
    weapon_t weapon;     // Arma en la caja (si aplica)
    armor_t armor;       // Armadura o casco en la caja (si aplica)
} box_t;

// Representa proyectiles lanzados por armas en el juego
typedef struct {
    position_t pos;  // Posición actual del proyectil
    uint8_t type;    // Tipo de proyectil (determina el sprite y efectos)
    bool is_active;  // Estado del proyectil (activo o inactivo)
} projectile_t;

// Representa el estado de un pato
typedef struct {
    position_t pos; // Posición actual del pato en el nivel
    uint8_t id;                 // ID único del pato para identificar al jugador
    bool faceLeft;          // Dirección hacia la que mira el pato
    bool isJumping;         // Estado de salto del pato
    bool isDucking;         // Estado de estar tirado al piso
    bool isFalling;         // Si el jugador está en caída libre
    bool isFlaping;         // Si el jugador está en salto
    uint8_t health;             // Salud actual del pato
    bool isAlive;             // Estado de vida del pato (vivo o muerto)
    uint8_t score;              // Puntaje acumulado del pato
    uint8_t color;              // Color asignado al pato   
    weapon_t equipped_weapon; // Arma equipada por el pato
    armor_t helmet;   // Armadura o casco equipado por el pato
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
} level_t;

// Estado global del juego
typedef struct {
    level_t level;
    uint8_t current_level;
    uint8_t round;         // Ronda actual en progreso
    uint8_t winning_score; // Puntaje necesario para ganar la partida
} game_state_t;

#endif /* __GAME_STATE_H__ */