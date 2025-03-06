#pragma once
#include "command.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"

namespace hmm {

class InstallCommand : public Command {
public:
    std::string getType() const override { return "install"; }
    
    void run(const std::vector<std::string>& args) override {
        Shell::ensureHmmJsonExists();
        Shell::createLocalHaxelibRepoIfNotExists();
        
        auto config = HmmConfig::readFromFile();
        for (const auto& library : config.dependencies) {
            install(library);
        }
        Shell::haxelibList();
    }

    std::string getUsage() const override {
        return "installs libraries listed in hmm.json";
    }

private:
    void install(const LibraryConfig& library) {
        std::visit([](const auto& dep) {
            if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, HaxelibDependency>) {
                Shell::haxelibInstall(dep.name, dep.version);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, GitDependency>) {
                Shell::haxelibGit(dep.name, dep.url, dep.ref, dep.dir);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, MercurialDependency>) {
                Shell::haxelibHg(dep.name, dep.url, dep.ref, dep.dir);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(dep)>, DevDependency>) {
                Shell::haxelibDev(dep.name, dep.path);
            }
        }, library);
    }
};

} 