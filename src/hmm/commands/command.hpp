#pragma once
#include <string>
#include <vector>

namespace hmm {

class Command {
public:
    virtual ~Command() = default;
    virtual std::string getType() const = 0;
    virtual void run(const std::vector<std::string>& args) = 0;
    virtual std::string getUsage() const = 0;
};

}