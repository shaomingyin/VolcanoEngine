//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <cassert>
#include <cstdint>
#include <chrono>
#include <memory>
#include <vector>
#include <stdexcept>
#include <filesystem>

#include <physfs.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <SFML/System/InputStream.hpp>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#   include <cassert>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr) ((void)0)
#endif

#define VOLCANO_CON(a, b) VOLCANO_CON_(a, b)
#define VOLCANO_CON_(a, b) a##b

#define VOLCANO_STR(x) VOLCANO_STR_(x)
#define VOLCANO_STR_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STR(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STR(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STR(VOLCANO_VERSION_PATCH)

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using Clock = std::chrono::steady_clock;
using ByteArray = std::vector<uint8_t>;

void to_json(nlohmann::json& json, const ByteArray& v);
void from_json(const nlohmann::json& json, ByteArray& v);
void to_json(nlohmann::json& json, const Clock::duration& v);
void from_json(const nlohmann::json& json, Clock::duration& v);
void to_json(nlohmann::json& json, const Clock::time_point& v);
void from_json(const nlohmann::json& json, Clock::time_point& v);

std::unique_ptr<sf::InputStream> PHYSFS_openInputStream(const std::filesystem::path& filepath);
ByteArray PHYSFS_readAll(const std::filesystem::path& filepath);

const std::string& appOrganization();
const std::string& appName();
void setAppInfo(const std::string& organization, const std::string& name);

VOLCANO_END

#endif // VOLCANO_COMMON_H    
