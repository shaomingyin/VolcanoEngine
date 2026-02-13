//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <cassert>
#include <cstdint>

#include <spdlog/spdlog.h>

#include <Volcano/Config.h>

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

template <typename... Args>
void logError(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::err, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logWarn(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logInfo(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::info, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logDebug(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
}

const std::string& appOrganization();
const std::string& appName();
void setAppInfo(const std::string& organization, const std::string& name);

VOLCANO_END

#endif // VOLCANO_COMMON_H    
