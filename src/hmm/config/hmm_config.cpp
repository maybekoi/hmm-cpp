#include "hmm_config.hpp"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace hmm {

using json = nlohmann::json;

HmmConfig HmmConfig::readFromFile() {
    std::filesystem::path configPath = getConfigPath();
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw ValidationError("Failed to open hmm.json");
    }

    json j;
    file >> j;
    return jsonToConfig(j);
}

void HmmConfig::addDependency(const LibraryConfig& config) {
    auto j = json::parse(std::ifstream(getConfigPath()));
    auto& deps = j["dependencies"];
    
    json depJson;
    std::visit([&depJson](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, HaxelibDependency>) {
            depJson = {
                {"type", "haxelib"},
                {"name", arg.name},
                {"version", arg.version.value_or("")}
            };
        } else if constexpr (std::is_same_v<T, GitDependency>) {
            depJson = {
                {"type", "git"},
                {"name", arg.name},
                {"url", arg.url},
                {"ref", arg.ref.value_or("master")},
                {"dir", arg.dir.value_or("")}
            };
        }
    }, config);

    deps.push_back(depJson);
    std::ofstream(getConfigPath()) << std::setw(2) << j << std::endl;
}

std::string HmmConfig::getConfigPath() {
    return (std::filesystem::current_path() / "hmm.json").string();
}

HmmConfig HmmConfig::jsonToConfig(const nlohmann::json& j) {
    HmmConfig config;
    
    if (!j.contains("dependencies") || !j["dependencies"].is_array()) {
        return config;
    }

    for (const auto& dep : j["dependencies"]) {
        if (!dep.contains("type") || !dep.contains("name")) {
            continue;
        }

        std::string type = dep["type"];
        std::string name = dep["name"];

        if (type == "haxelib") {
            std::optional<std::string> version;
            if (dep.contains("version") && !dep["version"].is_null() && !dep["version"].get<std::string>().empty()) {
                version = dep["version"].get<std::string>();
            }
            config.dependencies.push_back(HaxelibDependency{name, version});
        }
        else if (type == "git") {
            std::string url = dep["url"];
            std::optional<std::string> ref;
            std::optional<std::string> dir;
            
            if (dep.contains("ref") && !dep["ref"].is_null() && !dep["ref"].get<std::string>().empty()) {
                ref = dep["ref"].get<std::string>();
            }
            if (dep.contains("dir") && !dep["dir"].is_null() && !dep["dir"].get<std::string>().empty()) {
                dir = dep["dir"].get<std::string>();
            }
            config.dependencies.push_back(GitDependency{name, url, ref, dir});
        }
        else if (type == "hg") {
            std::string url = dep["url"];
            std::optional<std::string> ref;
            std::optional<std::string> dir;
            
            if (dep.contains("ref") && !dep["ref"].is_null() && !dep["ref"].get<std::string>().empty()) {
                ref = dep["ref"].get<std::string>();
            }
            if (dep.contains("dir") && !dep["dir"].is_null() && !dep["dir"].get<std::string>().empty()) {
                dir = dep["dir"].get<std::string>();
            }
            config.dependencies.push_back(MercurialDependency{name, url, ref, dir});
        }
        else if (type == "dev") {
            std::string path = dep["path"];
            config.dependencies.push_back(DevDependency{name, path});
        }
    }

    return config;
}

void HmmConfig::writeToFile(const HmmConfig& config) {
    json j;
    j["dependencies"] = json::array();

    for (const auto& dep : config.dependencies) {
        std::visit([&j](const auto& lib) {
            using T = std::decay_t<decltype(lib)>;
            json depJson;
            
            if constexpr (std::is_same_v<T, HaxelibDependency>) {
                depJson = {
                    {"type", "haxelib"},
                    {"name", lib.name},
                    {"version", lib.version.value_or("")}
                };
            } else if constexpr (std::is_same_v<T, GitDependency>) {
                depJson = {
                    {"type", "git"},
                    {"name", lib.name},
                    {"url", lib.url},
                    {"ref", lib.ref.value_or("master")},
                    {"dir", lib.dir.value_or("")}
                };
            } else if constexpr (std::is_same_v<T, DevDependency>) {
                depJson = {
                    {"type", "dev"},
                    {"name", lib.name},
                    {"path", lib.path}
                };
            }
            j["dependencies"].push_back(depJson);
        }, dep);
    }

    std::ofstream file(getConfigPath());
    file << std::setw(2) << j << std::endl;
}
} 