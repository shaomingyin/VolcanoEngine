//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <spdlog/spdlog.h>

#include <Eigen/Core>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#   include <cassert>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#define VOLCANO_CON(a, b) VOLCANO_CON_(a, b)
#define VOLCANO_CON_(a, b) a##b

#define VOLCANO_CON3(a, b, c) VOLCANO_CON3_(a, b, c)
#define VOLCANO_CON3_(a, b, c) a##b##c

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

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

VOLCANO_END

#endif // VOLCANO_COMMON_H
