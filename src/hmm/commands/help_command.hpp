#pragma once
#include "command.hpp"
#include <sstream>

namespace hmm {

class HelpCommand : public Command {
public:
    std::string getType() const override { 
        return "help"; 
    }
    
    void run(const std::vector<std::string>& args) override {
        std::stringstream ss;
        ss << "hmm - Haxe Module Manager\n\n"
           << "Available commands:\n"
           << "  init     - initializes the current directory for hmm usage\n"
           << "  install  - installs libraries listed in hmm.json\n"
           << "  git      - adds and installs a git-based dependency\n"
           << "  dev      - adds and installs a development dependency\n"
           << "  list     - lists all installed libraries\n"
           << "  help     - shows this help message\n"
           << "  set      - sets the version of a library\n"
           << "For more information about a command, run:\n"
           << "  hmm <command> --help";
        
        std::cout << ss.str() << std::endl;
    }

    std::string getUsage() const override {
        return "shows help information for hmm commands";
    }
};

}