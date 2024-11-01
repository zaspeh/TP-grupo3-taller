#include "clientprotocol.h"

void ClientProtocol::sendCommand(uint8_t num, bool &wasClosed) {
    this->sendUint8(this->client_identifier, wasClosed);
    this->sendUint8(num, wasClosed);
}

/*
typedef struct {
    float x;
    float y;
} position_t;

// Representa un arma en el juego
typedef struct {
    position_t pos;
    uint8_t type;    // Tipo de arma (ej.: pistola, escopeta, etc.)
    int ammo;        // Munición restante del arma
    bool is_equipped; // Indica si el arma está equipada por un jugador
} weapon_t;

// Representa una armadura o casco en el juego
typedef struct {
    position_t pos;
    uint8_t type;     // Tipo de armadura/casco
    bool is_equipped; // Indica si está equipada por un jugador
} armor_t;


typedef struct {
    position_t pos; // Posición actual del pato en el nivel
    uint8_t id;                 // ID único del pato para identificar al jugador
    bool faceLeft;          // Dirección hacia la que mira el pato
    bool isJumping;         // Estado de salto del pato
    bool isDucking;         // Estado de estar tirado al piso
    bool isFalling;         // Si el jugador está en caída libre
    bool isFlaping;         // Si el jugador está en salto
    uint8_t health;             // Salud actual del pato
    bool alive;             // Estado de vida del pato (vivo o muerto)
    uint8_t score;              // Puntaje acumulado del pato
    uint8_t color;              // Color asignado al pato   
    weapon_t equipped_weapon; // Arma equipada por el pato
    armor_t equipped_armor;   // Armadura o casco equipado por el pato
} duck_t;
*/

armor_t readArmor(){
    armor_t armor;
    armor.pos = readPosition();
    armor.type = recvUint8();
    armor.is_equipped = recvUint8() ? true : false;
    return armor;
}

weapon_t readWeapon(){
    weapon_t weapon;
    weapon.pos = readPosition();
    weapon.type = recvUint8();
    weapon.ammo = recvUint8();
    weapon.is_equipped = recvUint8() ? true : false;
    return weapon;
}

position_t readPosition(bool& wasClosed){
    position_t position;
    pos.start.x = recvFloat(wasClosed);
    pos.start.y = recvFloat(wasClosed);
    pos.start.w = recvFloat(wasClosed);
    pos.start.h = recvFloat(wasClosed);
    return position;
}

duck_t readDuck(bool& wasClosed){
    duck_t duck;
    duck.position = readPosition(wasClosed);
    duck.id = recvUint8(wasClosed);
    duck.faceLeft = recvUint8(wasClosed) ? true : false;
    duck.isJumping = recvUint8(wasClosed) ? true : false;
    duck.isDucking = recvUint8(wasClosed) ? true : false;
    duck.isFalling = recvUint8(wasClosed) ? true : false;
    duck.isFlaping = recvUint8(wasClosed) ? true : false;
    duck.health = recvUint8(wasClosed);
    duck.alive = recvUint8(wasClosed) ? true : false;
    duck.score = recvUint8(wasClosed);
    duck.color = recvUint8(wasClosed);
    duck.equipped_bow = readWeapon(wasClosed);
    duck.equipped_helmet = readArmor(wasClosed);
    return duck;
}

void readDucks(std::vector<duck_t>& ducks, bool &wasClosed) {
    uint8_t num_ducks = recvUint8(wasClosed);

    for (uint8_t i = 0; i < num_ducks; i++) {
        ducks[i] = readDuck(wasClosed);
    }
}

void readPlatforms(std::vector<Platform>& platforms, bool &wasClosed) {
    for (uint8_t i = 0; i < 50; i++) {
        platforms[i].start = readPosition(wasClosed);
        platforms[i].end = readPosition(wasClosed);
    }
}

void readSpawnPlaces(std::vector<SpawnPlace>& spawn_places, bool &wasClosed) {
    for (uint8_t i = 0; i < 20; i++) {
        spawn_places[i].pos = readPosition(wasClosed);
        spawn_places[i].is_active = recvUint8(wasClosed) ? true : false;
        spawn_places[i].weapon = readWeapon(wasClosed);
        spawn_places[i].armor = readArmor(wasClosed);
    }
}

void readBoxes(std::vector<Box>& boxes, bool &wasClosed) {
    for (uint8_t i = 0; i < num_boxes; i++) {
        boxes[i].pos = readPosition(wasClosed);
        boxes[i].is_explosive = recvUint8(wasClosed) ? true : false;
        boxes[i].is_destroyed = recvUint8(wasClosed) ? true : false;
        boxes[i].weapon = readWeapon(wasClosed);
        boxes[i].armor = readArmor(wasClosed);
    }
}

void readProjectiles(std::vector<Projectile>& projectiles, bool &wasClosed) {
    for (uint8_t i = 0; i < num_projectiles; i++) {
        projectiles[i].pos = readPosition(wasClosed);
        projectiles[i].type = recvUint8(wasClosed);
        projectiles[i].is_active = recvUint8(wasClosed) ? true : false;
    }
}

void ClientProtocol::readLevel(level_t level, bool &wasClosed) {
    readDucks(level.ducks, wasClosed);
    readPlatforms(level.platforms, wasClosed);
    readSpawnPlaces(level.spawn_places, wasClosed);
    readBoxes(level.boxes, wasClosed);
    readProjectiles(level.projectiles, wasClosed);
}

GameState ClientProtocol::readFromServer(bool &wasClosed) {
    GameState game_state;
    readLevel(game_state.level, wasClosed);
    game_state.current_level = recvUint8(wasClosed);    
    game_state.round = recvUint8(wasClosed);    
    game_state.winning_score = recvUint8(wasClosed);    
    return game_state;
}


ClientProtocol::ClientProtocol(Socket socket, uint8_t client_id)
        : Protocol(std::move(socket)),  
          client_identifier(client_id) {}  