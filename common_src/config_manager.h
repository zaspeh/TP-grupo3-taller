#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <yaml-cpp/yaml.h>
#include <string>

class ConfigManager {
public:
    static YAML::Node& getInstance() {
        static YAML::Node instance = YAML::LoadFile("/home/joseph/Escritorio/TALLER/TP-grupo3-taller/config.yaml");
        return instance;
    }

private:
    ConfigManager() = default; // Constructor privado para prevenir instancias.
};

#endif // CONFIG_MANAGER_H
