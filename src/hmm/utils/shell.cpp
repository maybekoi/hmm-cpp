#include "shell.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <array>
#include <memory>
#include <stdexcept>
#include "../errors/error.hpp"

namespace hmm {

std::string Shell::workingDirectory;
std::string Shell::hmmDirectory;
bool Shell::isWindows;
bool Shell::isQuiet;

int Shell::runCommand(const std::string& cmd, const std::vector<std::string>& args) {
    std::string fullCommand = cmd;
    for (const auto& arg : args) {
        fullCommand += " " + arg;
    }
    
    if (!isQuiet) {
        std::cout << "> " << fullCommand << std::endl;
    }
    
    return std::system(fullCommand.c_str());
}

void Shell::haxelibInstall(const std::string& name, 
                          const std::optional<std::string>& version,
                          const ShellOptions& options) {
    std::vector<std::string> args = {"install", name};
    if (version && !version->empty()) {
        args.push_back(*version);
    }
    
    int result = runCommand("haxelib", args);
    if (result != 0 && options.throwError) {
        throw std::runtime_error("Failed to install haxelib package: " + name);
    }
}

void Shell::createHmmJsonIfNotExists() {
    std::filesystem::path jsonPath = std::filesystem::path(workingDirectory) / "hmm.json";
    if (!std::filesystem::exists(jsonPath)) {
        std::ofstream file(jsonPath);
        file << "{\n  \"dependencies\": []\n}";
        file.close();
    }
}

void Shell::createLocalHaxelibRepoIfNotExists() {
    std::filesystem::path repoPath = std::filesystem::path(workingDirectory) / ".haxelib";
    if (!std::filesystem::exists(repoPath)) {
        std::filesystem::create_directory(repoPath);
        runCommand("haxelib", {"newrepo"});
    }
}

void Shell::init(const std::string& hmmDir, const std::string& workingDir, bool isWin, bool quiet) {
    hmmDirectory = hmmDir;
    workingDirectory = workingDir;
    isWindows = isWin;
    isQuiet = quiet;
}

void Shell::ensureHmmJsonExists() {
    createHmmJsonIfNotExists();
}

void Shell::haxelibDev(const std::string& name, const std::string& path, const ShellOptions& options) {
    std::vector<std::string> args = {"dev", name, path};
    int result = runCommand("haxelib", args);
    if (result != 0 && options.throwError) {
        throw SystemError("Failed to set up dev dependency: " + name);
    }
}

void Shell::haxelibGit(const std::string& name, const std::string& url,
                      const std::optional<std::string>& ref,
                      const std::optional<std::string>& dir,
                      const ShellOptions& options) {
    std::vector<std::string> args = {"git", name, url};
    if (ref && !ref->empty()) {
        args.push_back(*ref);
        if (dir && !dir->empty()) {
            args.push_back(*dir);
        }
    }
    
    int result = runCommand("haxelib", args);
    if (result != 0 && options.throwError) {
        throw SystemError("Failed to install git dependency: " + name);
    }
}

void Shell::haxelibHg(const std::string& name, const std::string& url,
                     const std::optional<std::string>& ref,
                     const std::optional<std::string>& dir,
                     const ShellOptions& options) {
    std::vector<std::string> args = {"hg", name, url};
    if (ref && !ref->empty()) {
        args.push_back(*ref);
        if (dir && !dir->empty()) {
            args.push_back(*dir);
        }
    }
    
    int result = runCommand("haxelib", args);
    if (result != 0 && options.throwError) {
        throw SystemError("Failed to install mercurial dependency: " + name);
    }
}

void Shell::haxelibList(const ShellOptions& options) {
    int result = runCommand("haxelib", {"list"});
    if (result != 0 && options.throwError) {
        throw SystemError("Failed to list haxelib packages");
    }
}

std::string Shell::readCommand(const std::string& cmd, const std::vector<std::string>& args) {
    std::array<char, 128> buffer;
    std::string result;
    
    std::string fullCmd = cmd;
    for (const auto& arg : args) {
        fullCmd += " " + arg;
    }
    
    #ifdef _WIN32
        FILE* pipe = _popen(fullCmd.c_str(), "r");
    #else
        FILE* pipe = popen(fullCmd.c_str(), "r");
    #endif

    if (!pipe) {
        throw SystemError("popen() failed!");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    
    #ifdef _WIN32
        _pclose(pipe);
    #else
        pclose(pipe);
    #endif

    while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
        result.pop_back();
    }
    
    return result;
}

}