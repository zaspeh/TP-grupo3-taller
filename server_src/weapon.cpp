#include "weapon.h"

YAML::Node& config = ConfigManager::getInstance();

std::map<int, float> weaponRecoil = {
    {GRENADE_WEAPON, config["weapons"]["grenade"]["recoil"].as<float>()},
    {BANANA_WEAPON, config["weapons"]["banana"]["recoil"].as<float>()},
    {DARTGUN_WEAPON, config["weapons"]["dartgun"]["recoil"].as<float>()},
    {AK_47_WEAPON, config["weapons"]["ak47"]["recoil"].as<float>()},
    {PEWPEWLASER_WEAPON, config["weapons"]["pewpewlaser"]["recoil"].as<float>()},
    {LASERRIFLE_WEAPON, config["weapons"]["laserrifle"]["recoil"].as<float>()},
    {COWBOY_WEAPON, config["weapons"]["cowboy"]["recoil"].as<float>()},
    {MAGNUM_WEAPON, config["weapons"]["magnum"]["recoil"].as<float>()},
    {SHOTGUN_WEAPON, config["weapons"]["shotgun"]["recoil"].as<float>()},
    {SNIPER_WEAPON, config["weapons"]["sniper"]["recoil"].as<float>()}
};

std::map<int, uint8_t> ammoForWeapons = {
    {GRENADE_WEAPON, config["weapons"]["grenade"]["ammo"].as<uint8_t>()},
    {BANANA_WEAPON, config["weapons"]["banana"]["ammo"].as<uint8_t>()},
    {DARTGUN_WEAPON, config["weapons"]["dartgun"]["ammo"].as<uint8_t>()},
    {AK_47_WEAPON, config["weapons"]["ak47"]["ammo"].as<uint8_t>()},
    {PEWPEWLASER_WEAPON, config["weapons"]["pewpewlaser"]["ammo"].as<uint8_t>()},
    {LASERRIFLE_WEAPON, config["weapons"]["laserrifle"]["ammo"].as<uint8_t>()},
    {COWBOY_WEAPON, config["weapons"]["cowboy"]["ammo"].as<uint8_t>()},
    {MAGNUM_WEAPON, config["weapons"]["magnum"]["ammo"].as<uint8_t>()},
    {SHOTGUN_WEAPON, config["weapons"]["shotgun"]["ammo"].as<uint8_t>()},
    {SNIPER_WEAPON, config["weapons"]["sniper"]["ammo"].as<uint8_t>()}
};
