#pragma once
#include "command.hpp"
#include "../errors/error.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"

namespace hmm {

class GitCommand : public Command {
public:
    std::string getType() const override { return "git"; }
    
    void run(const std::vector<std::string>& args) override {
        Shell::ensureHmmJsonExists();
        Shell::createLocalHaxelibRepoIfNotExists();

        if (args.size() < 2 || args.size() > 4) {
            throw ValidationError("git command requires 2, 3, or 4 arguments (<name> <url> [ref] [dir])");
        }

        std::string name = args[0];
        std::string url = args[1];
        std::optional<std::string> ref = "master";
        std::optional<std::string> dir;

        if (args.size() >= 3 && !args[2].empty()) {
            ref = args[2];
        }

        if (args.size() == 4 && !args[3].empty()) {
            dir = args[3];
        }

        GitDependency dep{name, url, ref, dir};
        HmmConfig::addDependency(dep);
        Shell::haxelibGit(name, url, ref, dir);
        Shell::haxelibList();
    }

    std::string getUsage() const override {
        return "adds and installs a git-based dependency";
    }
};

}