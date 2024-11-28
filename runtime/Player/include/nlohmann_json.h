//
//
#ifndef NLOHMANN_JSON_H
#define NLOHMANN_JSON_H

#include <format>
#include <string>
#include <stdexcept>

#include <Eigen.h>
#include <physfs.h>
#include <nlohmann/json.hpp>

namespace nlohmann {
    static inline bool parseBool(const json& j, const char* k, bool def = false) {
        auto it = j.find(k);
        if (it != j.end() && it->is_boolean()) {
            return it->get<bool>();
        }
        return def;
    }

    static inline std::string parseString(const json& j, const char* k, std::string def = std::string()) {
        auto it = j.find(k);
        if (it != j.end() && it->is_string()) {
            return it->get<std::string>();
        }
        return def;
    }

    template <typename T, size_t D>
    static inline auto parseVector(const json& j, const char* k) {
        auto it = j.find(k);
        if (it != j.end()) {
            if (!it->is_array()) {
                throw std::runtime_error(std::format("Key {} is not an array.", k));
            }
            if (it->size() != D) {
                throw std::runtime_error(std::format("Key {} is not a {}D vector.", k, D));
            }
        }
        return it;
    }

    template <typename T>
    static inline Eigen::Vector2<T> parseVector2(const json& j, const char* k, Eigen::Vector2<T> def) {
        auto it = parseVector<T, 2>(j, k);
        if (it != j.end()) {
            return Eigen::Vector2<T>(
                it->at(0).get<T>(),
                it->at(1).get<T>());
        }
        return def;
    }

    static inline Eigen::Vector2i parseVector2i(const json& j, const char* k, Eigen::Vector2i def = Eigen::Vector2i()) {
        return parseVector2<int>(j, k, def);
    }

    static inline Eigen::Vector2f parseVector2f(const json& j, const char* k, Eigen::Vector2f def = Eigen::Vector2f()) {
        return parseVector2<float>(j, k, def);
    }

    static inline Eigen::Vector2d parseVector2d(const json& j, const char* k, Eigen::Vector2d def = Eigen::Vector2d()) {
        return parseVector2<double>(j, k, def);
    }

    template <typename T>
    static inline Eigen::Vector3<T> parseVector3(const json& j, const char* k, Eigen::Vector3<T> def) {
        auto it = parseVector<T, 3>(j, k);
        if (it != j.end()) {
            return Eigen::Vector3<T>(
                it->at(0).get<T>(),
                it->at(1).get<T>(),
                it->at(2).get<T>());
        }
        return def;
    }

    static inline Eigen::Vector3i parseVector3i(const json& j, const char* k, Eigen::Vector3i def = Eigen::Vector3i()) {
        return parseVector3<int>(j, k, def);
    }

    static inline Eigen::Vector3f parseVector3f(const json& j, const char* k, Eigen::Vector3f def = Eigen::Vector3f()) {
        return parseVector3<float>(j, k, def);
    }

    static inline Eigen::Vector3d parseVector3d(const json& j, const char* k, Eigen::Vector3d def = Eigen::Vector3d()) {
        return parseVector3<double>(j, k, def);
    }

    template <typename T>
    static inline Eigen::Vector4<T> parseVector4(const json& j, const char* k, Eigen::Vector4<T> def) {
        auto it = parseVector<T, 4>(j, k);
        if (it != j.end()) {
            return Eigen::Vector4<T>(
                it->at(0).get<T>(),
                it->at(1).get<T>(),
                it->at(2).get<T>(),
                it->at(3).get<T>());
        }
        return def;
    }

    static inline Eigen::Vector4i parseVector4i(const json& j, const char* k, Eigen::Vector4i def = Eigen::Vector4i()) {
        return parseVector4<int>(j, k, def);
    }

    static inline Eigen::Vector4f parseVector4f(const json& j, const char* k, Eigen::Vector4f def = Eigen::Vector4f()) {
        return parseVector4<float>(j, k, def);
    }

    static inline Eigen::Vector4d parseVector4d(const json& j, const char* k, Eigen::Vector4d def = Eigen::Vector4d()) {
        return parseVector4<double>(j, k, def);
    }

    static inline json parseFromPhysFS(std::string physfs_path) {
        auto file = PHYSFS_openRead(physfs_path.c_str());
        if (file == nullptr) {
            throw std::runtime_error(std::format("Failed to open manifest {}: {}", physfs_path, PHYSFS_getLastError()));
        }
        auto size = PHYSFS_fileLength(file);
        if (size < 1) {
            throw std::runtime_error(std::format("Empty manifest file {}: {}", physfs_path));
        }
        std::vector<uint8_t> buff(size);
        if (PHYSFS_readBytes(file, buff.data(), size) != size) {
            throw std::runtime_error(std::format("Failed to read manifest file {}: {}", physfs_path, PHYSFS_getLastError()));
        }
        return nlohmann::json::parse(buff);
    }
}

#endif // NLOHMANN_JSON_H
