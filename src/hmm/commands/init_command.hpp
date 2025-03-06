#pragma once
#include "command.hpp"
#include "../utils/shell.hpp"
#include "../config/hmm_config.hpp"

namespace hmm {

class InitCommand : public Command {
public:
    std::string getType() const override {
        return "init";
    }

    void run(const std::vector<std::string>& args) override {
        Shell::createLocalHaxelibRepoIfNotExists();
        Shell::createHmmJsonIfNotExists();
    }

    std::string getUsage() const override {
        return "initializes the current working directory for hmm usage";
    }
};

} 