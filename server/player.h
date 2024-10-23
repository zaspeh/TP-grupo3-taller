#include "../common_src/protocol.h"
#include "../common_src/thead.h"
#include "../common_src/queue.h"
#include "position.h"

class Player {
private:
    int id; 
    std::string name;
    Position pos;
    int health;
    bool isAlive;
    Weapon weapon;
    bool hasWeapon;
    std::string currentAction;
    int victoriesAchieved;
    Queue queue;
    Protocol protocol;
    bool was_closed;

public:
    Player() = default;
    ~Player() = default;
    Player(Player const&) = default;
    Player& operator=(Player const&) = default;

class PlayerState {
private:
    int id; 
    Position pos;
    int health;
    bool hasWeapon;
    bool isAlive;
    std::string currentAction;

public:
    // Constructor
    PlayerState(int playerId);

    // Getters
    int getId() const;
    Position getPosition() const;
    int getHealth() const;
    bool isPlayerAlive() const;
    bool playerHasWeapon() const;
    std::string getCurrentAction() const;

    // Setters
    void setPosition(int x, int y);
    void setHealth(int hp);
    void setIsAlive(bool alive);
    void setHasWeapon(bool hasWeapon);
    void setCurrentAction(const std::string& action);
};