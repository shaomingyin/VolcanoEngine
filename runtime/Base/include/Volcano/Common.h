//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <cstdlib>
#include <cstdint>
#include <chrono>

#include <Eigen/Core>
#include <spdlog/spdlog.h>

#include <Volcano/Config.h>

#ifdef __cplusplus
#   define VOLCANO_C extern "C"
#   define VOLCANO_C_BEGIN extern "C" {
#   define VOLCANO_C_END }
#else
#   define VOLCANO_C
#   define VOLCANO_C_BEGIN
#   define VOLCANO_C_END
#endif

#ifdef VOLCANO_DEBUG
#   include <cassert>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#if defined(__GNUC__)
#   define VOLCANO_EXPORT
#   define VOLCANO_IMPORT
#   define VOLCANO_FORCE_INLINE __inline __attribute__ ((__always_inline__))
#   define VOLCANO_LIKELY(expr) __builtin_expect((expr), 1)
#   define VOLCANO_UNLIKELY(expr) __builtin_expect((expr), 0)
#elif defined(_MSC_VER)
#   define VOLCANO_EXPORT __declspec(dllexport)
#   define VOLCANO_IMPORT __declspec(dllimport)
#   define VOLCANO_FORCE_INLINE __forceinline
#   define VOLCANO_LIKELY(expr) (expr)
#   define VOLCANO_UNLIKELY(expr) (expr)
#else
#   error Unknown compiler.
#endif

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

template <typename... Args>
void log(spdlog::level::level_enum level, spdlog::format_string_t<Args...> fmt, Args&&... args) {
    spdlog::log(level, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logError(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    log(spdlog::level::err, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logWarning(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logInfo(spdlog::format_string_t<Args...> fmt, Args&&... args) {
    log(spdlog::level::info, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logDebug(spdlog::format_string_t<Args...> fmt, Args&&... args) {
#ifdef VOLCANO_DEBUG
    log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
#endif
}

VOLCANO_END

#endif // VOLCANO_COMMON_H
