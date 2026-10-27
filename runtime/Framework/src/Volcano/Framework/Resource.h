#ifndef VOLCANO_FRAMEWORK_RESOURCE_H
#define VOLCANO_FRAMEWORK_RESOURCE_H

#include <filesystem>
#include <stdexcept>
#include <string>

#include <Volcano/PhysFile.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

inline std::filesystem::path resourcePath(const std::filesystem::path& path) {
    const auto text = path.generic_string();
    if (text.empty() || path.is_absolute() || text.find('\0') != std::string::npos ||
        text.find('\\') != std::string::npos || text.find(':') != std::string::npos) {
        throw std::invalid_argument("Invalid resource path: " + text);
    }
    for (const auto& part : path) {
        if (part == "..") {
            throw std::invalid_argument("Resource path cannot contain '..': " + text);
        }
    }
    const auto result = path.lexically_normal();
    if (result == "." || result.filename().empty()) {
        throw std::invalid_argument("Resource path must name a file: " + text);
    }
    return result;
}

template <typename Resource>
Resource readResource(const std::filesystem::path& path) {
    try {
        PhysFile file(path);
        return Resource::read(file);
    } catch (const std::exception& error) {
        throw std::runtime_error("Cannot load resource '" + path.generic_string() + "': " + error.what());
    }
}

VOLCANO_FRAMEWORK_END

#endif
