#ifndef __LEVEL_H__
#define __LEVEL_H__

class Level
{
private:
    std::vector<std::shared_ptr<SpawnPoint>> spawnPoints;
    std::vector<std::shared_ptr<Box>> boxes;
public:
    Level();
    ~Level();

    std::vector<std::shared_ptr<SpawnPoint>> getSpawnPoints();
    std::vector<std::shared_ptr<Box>> getBoxes();
    std::shared_ptr<level> getLevel();
};



#endif // __LEVEL_H__