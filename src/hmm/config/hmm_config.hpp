#pragma once
#include "../types.hpp"
#include <string>
#include <vector>
#include "../errors/error.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <variant>

namespace hmm {

struct HmmConfig {
    std::vector<LibraryConfig> dependencies;
    
    static HmmConfig readFromFile();
    static void writeToFile(const HmmConfig& config);
    static void addDependency(const LibraryConfig& config);
    static void removeDependency(const std::string& name);
    static void save();

private:
    static std::string getConfigPath();
    static nlohmann::json configToJson(const HmmConfig& config);
    static HmmConfig jsonToConfig(const nlohmann::json& json);
};

}