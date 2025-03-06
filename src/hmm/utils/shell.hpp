#pragma once
#include <string>
#include <vector>
#include "../types.hpp"

namespace hmm {

struct ShellOptions {
    bool log = true;
    bool throwError = true;
};

class Shell {
public:
    static void init(const std::string& hmmDir, const std::string& workingDir, bool isWin, bool quiet);
    
    static void ensureHmmJsonExists();
    static void createHmmJsonIfNotExists();
    static void createLocalHaxelibRepoIfNotExists();
    static void removeLocalHaxelibRepoIfExists();
    
    static void haxelibInstall(const std::string& name, 
                              const std::optional<std::string>& version,
                              const ShellOptions& options = {});
    
    static void haxelibGit(const std::string& name,
                          const std::string& url,
                          const std::optional<std::string>& ref,
                          const std::optional<std::string>& dir,
                          const ShellOptions& options = {});
                          
    static void haxelibHg(const std::string& name,
                         const std::string& url,
                         const std::optional<std::string>& ref,
                         const std::optional<std::string>& dir,
                         const ShellOptions& options = {});
                         
    static void haxelibDev(const std::string& name,
                          const std::string& path,
                          const ShellOptions& options = {});
                          
    static void haxelibList(const ShellOptions& options = {});
    static void haxelibRemove(const std::string& name, const ShellOptions& options = {});
    
    static bool isAlreadyInstalled(const LibraryConfig& config, const ShellOptions& options = {});
    
    static std::string workingDirectory;
    static std::string hmmDirectory;
    static bool isWindows;
    static bool isQuiet;

    static std::string readCommand(const std::string& cmd, const std::vector<std::string>& args);

private:
    static int runCommand(const std::string& cmd, const std::vector<std::string>& args);
};

}