#pragma once
#include "command.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"
#include <filesystem>

namespace hmm {

class ListCommand : public Command {
public:
    std::string getType() const override { return "list"; }
    
    void run(const std::vector<std::string>& args) override {
        auto config = HmmConfig::readFromFile();
        
        for (const auto& library : config.dependencies) {
            std::visit([this](const auto& dep) {
                if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, GitDependency>) {
                    printGitInfo(dep.name, dep.url, dep.ref);
                } else if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, HaxelibDependency>) {
                    printHaxelibInfo(dep.name, dep.version);
                } else if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, DevDependency>) {
                    printDevInfo(dep.name, dep.path);
                }
            }, library);
        }
    }

    std::string getUsage() const override {
        return "lists all dependencies and their detailed version information";
    }

private:
    void printGitInfo(const std::string& name, const std::string& url, const std::optional<std::string>& ref) {
        std::cout << name << ": [git] ";
        
        if (std::filesystem::exists(".haxelib/" + name + "/git/.git")) {
            auto currentRef = getCurrentGitRef(name);
            std::cout << url << " @ " << currentRef;
            
            if (ref && *ref != currentRef) {
                std::cout << " (specified: " << *ref << ")";
            }
        } else {
            std::cout << url;
            if (ref) std::cout << " @ " << *ref;
            std::cout << " (not installed)";
        }
        std::cout << std::endl;
    }

    std::string getCurrentGitRef(const std::string& name) {
        auto cwd = std::filesystem::current_path();
        std::filesystem::current_path(".haxelib/" + name + "/git");
        
        auto tag = Shell::readCommand("git", {"tag", "--points-at", "HEAD"});
        auto hash = Shell::readCommand("git", {"rev-parse", "--short", "HEAD"});
        
        std::filesystem::current_path(cwd);
        
        if (!tag.empty()) return tag;
        return hash;
    }

    void printHaxelibInfo(const std::string& name, const std::optional<std::string>& version) {
        std::cout << name << ": ";
        if (version) {
            std::cout << *version;
        } else {
            std::cout << "(latest)";
        }
        std::cout << std::endl;
    }

    void printDevInfo(const std::string& name, const std::string& path) {
        std::cout << name << ": [dev] " << path << std::endl;
    }
};

}