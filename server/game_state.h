#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string>

// Clase que representa el estado de un jugador


// Clase que representa el estado de un objeto en el juego
class GameObject {
private:
    std::string type;
    Position pos;
    bool isActive;

public:
    GameObject(const std::string& type, int x, int y);

    std::string getType() const;
    Position getPosition() const;
    bool getIsActive() const;

    void setActive(bool active);
    void setPosition(int x, int y);
};

// Clase que representa un proyectil
class Projectile {
private:
    Position pos;
    int velocityX;
    int velocityY;
    bool isActive;

public:
    Projectile(int x, int y, int velX, int velY);

    Position getPosition() const;
    bool getIsActive() const;
    void updatePosition();  // Actualiza la posición en base a la velocidad
};

// Clase GameState que encapsula el estado del juego completo
class GameState {
private:
    std::vector<PlayerState> players;
    std::vector<GameObject> objects;
    std::vector<Projectile> projectiles;
    int roundNumber;
    bool isGameActive;

public:
    GameState();

    // Métodos para manipular jugadores
    void addPlayer(int playerId);
    PlayerState* getPlayerById(int playerId);

    // Métodos para manipular objetos
    void addObject(const std::string& type, int x, int y);
    GameObject* getObjectAtPosition(int x, int y);

    // Métodos para manejar proyectiles
    void addProjectile(int x, int y, int velX, int velY);
    void updateProjectiles();

    // Métodos para el estado global del juego
    int getRoundNumber() const;
    bool getIsGameActive() const;
    void setRoundNumber(int round);
    void setGameActive(bool active);
};

#endif
