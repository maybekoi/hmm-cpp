#pragma once
#include <stdexcept>
#include <string>

namespace hmm {

class ValidationError : public std::runtime_error {
public:
    explicit ValidationError(const std::string& message) 
        : std::runtime_error(message) {}
};

class SystemError : public std::runtime_error {
public:
    explicit SystemError(const std::string& message) 
        : std::runtime_error(message) {}
};

}