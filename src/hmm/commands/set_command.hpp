#pragma once
#include "command.hpp"
#include "../errors/error.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"

namespace hmm {

class SetCommand : public Command {
public:
    std::string getType() const override { return "set"; }
    
    void run(const std::vector<std::string>& args) override {
        if (args.size() < 1 || args.size() > 2) {
            throw ValidationError("set command requires 1 or 2 arguments (<name> [version])");
        }

        std::string name = args[0];
        std::optional<std::string> version;
        
        if (args.size() == 2 && !args[1].empty()) {
            version = args[1];
        }

        auto config = HmmConfig::readFromFile();
        bool found = false;

        std::vector<LibraryConfig> newDeps;
        for (const auto& lib : config.dependencies) {
            std::visit([&](const auto& dep) {
                using T = std::decay_t<decltype(dep)>;
                if constexpr (std::is_same_v<T, HaxelibDependency>) {
                    if (dep.name == name) {
                        newDeps.push_back(HaxelibDependency{name, version});
                        found = true;
                    } else {
                        newDeps.push_back(dep);
                    }
                } else {
                    newDeps.push_back(dep);
                }
            }, lib);
        }

        if (!found) {
            throw ValidationError("Library not found in hmm.json: " + name);
        }

        config.dependencies = newDeps;
        HmmConfig::writeToFile(config);

        Shell::haxelibInstall(name, version);
        Shell::haxelibList();
    }

    std::string getUsage() const override {
        return "changes the version of a haxelib dependency\n"
               "\n"
               "usage: hmm set <name> [version]\n"
               "\n"
               "arguments:\n"
               "- name - the name of the library (required)\n"
               "- version - the new version (optional)\n"
               "\n"
               "example:\n"
               "  hmm set flixel 5.0.0";
    }
};
}