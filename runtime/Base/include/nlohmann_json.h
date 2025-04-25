//
//
#ifndef NLOHMANN_JSON_H
#define NLOHMANN_JSON_H

#include <string>

#include <nlohmann/json.hpp>
#include <SDL3/SDL_storage.h>

namespace nlohmann {
    json loadFromStorage(SDL_Storage* storage, const char* path);
    json findByPath(const json& j, const std::string& path);

    template <typename T>
    inline T get(const json& j, const std::string& path, T def) {
        try {
            return findByPath(j, path).get<T>();
        } catch (...) {
        }
        return def;
    }

    inline bool getBool(const json& j, const std::string& path, bool def = false) {
        return get<bool>(j, path, def);
    }

    inline int getInt(const json& j, const std::string& path, int def = 0) {
        return get<int>(j, path, def);
    }

    inline float getFloat(const json& j, const std::string& path, float def = 0.0f) {
        return get<float>(j, path, def);
    }

    inline std::string getString(const json& j, const std::string& path, std::string def = std::string()) {
        return get<std::string>(j, path, std::forward<std::string>(def));
    }
}

#endif // NLOHMANN_JSON_H
