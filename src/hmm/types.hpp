#pragma once
#include <string>
#include <optional>
#include <variant>
#include <vector>

namespace hmm {

struct HaxelibDependency {
    std::string name;
    std::optional<std::string> version;
};

struct GitDependency {
    std::string name;
    std::string url;
    std::optional<std::string> ref;
    std::optional<std::string> dir;
};

struct MercurialDependency {
    std::string name;
    std::string url;
    std::optional<std::string> ref;
    std::optional<std::string> dir;
};

struct DevDependency {
    std::string name;
    std::string path;
};

using LibraryConfig = std::variant<
    HaxelibDependency,
    GitDependency, 
    MercurialDependency,
    DevDependency
>;

}