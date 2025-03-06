#pragma once
#include <memory>
#include <vector>
#include "commands/command.hpp"
#include "commands/help_command.hpp"
#include "commands/init_command.hpp"
#include "commands/install_command.hpp"
#include "commands/git_command.hpp"
#include "commands/dev_command.hpp"
#include "commands/list_command.hpp"
#include "commands/set_command.hpp"
#include "utils/shell.hpp"
#include "errors/error.hpp"

namespace hmm {

class HmmApp {
private:
    std::vector<std::unique_ptr<Command>> commands;
    bool isQuiet;

public:
    HmmApp() : isQuiet(false) {
        registerCommands();
    }

    void initialize(const std::string& workingDir, bool quiet) {
        Shell::init(std::filesystem::current_path().string(), workingDir, 
            #ifdef _WIN32
                true
            #else
                false
            #endif
        , quiet);
        isQuiet = quiet;
    }

    void run(int argc, char* argv[]) {
        if (argc < 2) {
            throw ValidationError("no command given");
        }

        std::string commandName = argv[1];
        std::vector<std::string> args;
        for (int i = 2; i < argc; i++) {
            args.push_back(argv[i]);
        }

        auto cmd = findCommand(commandName);
        if (!cmd) {
            throw ValidationError("unrecognized command: " + commandName);
        }

        cmd->run(args);
    }

private:
    void registerCommands() {
        commands.emplace_back(std::make_unique<HelpCommand>());
        commands.emplace_back(std::make_unique<InitCommand>());
        commands.emplace_back(std::make_unique<InstallCommand>());
        commands.emplace_back(std::make_unique<GitCommand>());
        commands.emplace_back(std::make_unique<DevCommand>());
        commands.emplace_back(std::make_unique<ListCommand>());
        commands.emplace_back(std::make_unique<SetCommand>());
    }

    Command* findCommand(const std::string& type) {
        for (const auto& cmd : commands) {
            if (cmd->getType() == type) {
                return cmd.get();
            }
        }
        return nullptr;
    }
};

}