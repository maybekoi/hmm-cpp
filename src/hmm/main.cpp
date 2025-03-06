#include <iostream>
#include <filesystem>
#include "commands/command.hpp"
#include "commands/init_command.hpp"
#include "commands/install_command.hpp"
#include "commands/git_command.hpp"
#include "utils/shell.hpp"
#include "errors/error.hpp"
#include "app.hpp"

using namespace hmm;

int main(int argc, char* argv[]) {
    try {
        HmmApp app;
        app.initialize(std::filesystem::current_path().string(), false);
        app.run(argc, argv);
        return 0;
    } catch (const ValidationError& e) {
        std::cerr << "Validation error: " << e.what() << std::endl;
        std::cout << "Use \"hmm help\" to see usage" << std::endl;
        return 1;
    } catch (const SystemError& e) {
        std::cerr << "Execution error: " << e.what() << std::endl;
        std::cout << "Use \"hmm help\" to see usage" << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        std::cout << "Use \"hmm help\" to see usage" << std::endl;
        return 1;
    }
}