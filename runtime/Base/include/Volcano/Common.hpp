//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <Eigen/Core>
#include <spdlog/spdlog.h>

#include <Volcano/Common.h>

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

#endif // VOLCANO_COMMON_HPP    
