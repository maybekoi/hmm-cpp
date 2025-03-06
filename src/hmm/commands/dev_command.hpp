#pragma once
#include "command.hpp"
#include "../errors/error.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"

namespace hmm {

class DevCommand : public Command {
public:
    std::string getType() const override { return "dev"; }
    
    void run(const std::vector<std::string>& args) override {
        Shell::ensureHmmJsonExists();
        Shell::createLocalHaxelibRepoIfNotExists();

        if (args.size() != 2) {
            throw ValidationError("dev command requires 2 arguments: <name> and <path>");
        }

        std::string name = args[0];
        std::string path = args[1];

        DevDependency dep{name, path};
        HmmConfig::addDependency(dep);
        Shell::haxelibDev(name, path);
        Shell::haxelibList();
    }

    std::string getUsage() const override {
        return "adds and installs a development dependency";
    }
};

} 