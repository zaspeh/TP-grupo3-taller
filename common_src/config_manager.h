#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <yaml-cpp/yaml.h>
#include <string>

class ConfigManager {
public:
    static YAML::Node& getInstance() {
        static YAML::Node instance = YAML::LoadFile("./config.yaml");
        return instance;
    }

private:
    ConfigManager() = default;
};

#endif // CONFIG_MANAGER_H
