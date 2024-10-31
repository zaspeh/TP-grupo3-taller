#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <stdbool.h>
#include <stdint.h>

#define MAX_DUCKS 10
#define MAX_PROJECTILES 100
#define MAX_ITEMS 50
#define MAX_LEVELS 5
#define MAX_PLATFORMS 50
#define MAX_SPAWN_PLACES 20
#define MAX_BOXES 20

typedef struct {
    float x;
    float y;
} position_t;

// Representa plataformas en el escenario
typedef struct {
    position_t start; // Posición inicial de la plataforma
    position_t end;   // Posición final de la plataforma
} platform_t;

// Representa un arma en el juego
typedef struct {
    uint8_t type;    // Tipo de arma (ej.: pistola, escopeta, etc.)
    int ammo;        // Munición restante del arma
    bool is_equipped; // Indica si el arma está equipada por un jugador
} weapon_t;

// Representa una armadura o casco en el juego
typedef struct {
    uint8_t type;     // Tipo de armadura/casco
    bool is_equipped; // Indica si está equipada por un jugador
} armor_t;

// Representa puntos de aparición donde aparecen armas y armaduras
typedef struct {
    position_t pos;    // Posición del spawn place en el nivel
    bool is_active;    // Si actualmente hay un item disponible en el spawn place
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
    int id;                 // ID único del pato para identificar al jugador
    position_t pos; // Posición actual del pato en el nivel
    bool faceLeft;          // Dirección hacia la que mira el pato
    bool isJumping;         // Estado de salto del pato
    bool isDucking;         // Estado de estar tirado al piso
    int health;             // Salud actual del pato
    bool alive;             // Estado de vida del pato (vivo o muerto)
    int score;              // Puntaje acumulado del pato
    weapon_t equipped_weapon; // Arma equipada por el pato
    armor_t equipped_armor;   // Armadura o casco equipado por el pato
} duck_t;

// Representa un nivel completo con todos sus elementos
typedef struct {
    duck_t ducks[MAX_DUCKS];
    int num_ducks;
    platform_t platforms[MAX_PLATFORMS];
    int num_platforms;
    spawn_place_t spawn_places[MAX_SPAWN_PLACES];
    int num_spawn_places;
    box_t boxes[MAX_BOXES];
    int num_boxes;
    projectile_t projectiles[MAX_PROJECTILES];
    int num_projectiles;
} level_t;

// Estado global del juego
typedef struct {
    level_t levels[MAX_LEVELS];
    int current_level;
    int round;         // Ronda actual en progreso
    int winning_score; // Puntaje necesario para ganar la partida
} GameState;

#endif /* __GAME_STATE_H__ */