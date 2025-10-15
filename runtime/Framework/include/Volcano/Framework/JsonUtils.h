//
//
#ifndef VOLCANO_FRAMEWORK_JSONUTILS_H
#define VOLCANO_FRAMEWORK_JSONUTILS_H

#include <fstream>

#include <Volcano/Version.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

namespace JsonUtils {
    std::vector<uint8_t> readPhysFile(const char* filepath);
    nlohmann::json find(const nlohmann::json& json, const char* jsonpath);

    inline nlohmann::json loadFromFile(const char* filepath) {
        VOLCANO_ASSERT(filepath != nullptr);
        return nlohmann::json::parse(std::ifstream(filepath));
    }

    inline nlohmann::json loadFromPhysFile(const char* filepath) {
        VOLCANO_ASSERT(filepath != nullptr);
        return nlohmann::json::parse(readPhysFile(filepath));
    }

    template <typename T>
    T value(const nlohmann::json& json, const char* jsonpath, T def = T()) noexcept {
        VOLCANO_ASSERT(jsonpath != nullptr);
        try {
            return find(json, jsonpath).get<T>();
        } catch (...) {
        }
        return def;
    }

    inline VersionNumber versionValue(const nlohmann::json& json, const char* jsonpath, VersionNumber def) noexcept {
        VOLCANO_ASSERT(jsonpath != nullptr);
        try {
            auto j = find(json, jsonpath);
            return VersionNumber(
                j.at(0).get<int>(),
                j.at(1).get<int>(),
                j.at(2).get<int>()
            );
        } catch (...) {
        }
        return def;
    }
}

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_JSONUTILS_H
