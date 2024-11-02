#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <vector>
#include <memory>
#include "../common_src/game_state.h"

class Level
{
private:
    level_t level;
    std::vector<std::shared_ptr<spawn_place_t>> spawnPoints;
    std::vector<std::shared_ptr<box_t>> boxes;
    
public:
    Level() {} ;

    level_t& getState() { return level; }

    ~Level() {};

    std::vector<std::shared_ptr<spawn_place_t>> getSpawnPoints() { return spawnPoints; }
    std::vector<std::shared_ptr<box_t>> getBoxes() { return boxes; }
    level_t getLevel() { return level; }
};


#endif // __LEVEL_H__