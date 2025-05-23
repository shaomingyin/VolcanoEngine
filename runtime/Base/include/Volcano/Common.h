//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <cstdlib>
#include <cstdint>
#include <cstdarg>

#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <format>
#include <stdexcept>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <Volcano/Config.h>

#include <async++.h>
#include <rttr/type>
#include <SDL3/SDL_log.h>

#ifdef VOLCANO_PROFILE
#   include <microprofile.h>
#endif

#ifdef VOLCANO_DEBUG
#   include <SDL3/SDL_assert.h>
#   define VOLCANO_ASSERT(expr) SDL_assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#ifdef __cplusplus
#   define VOLCANO_C extern "C"
#   define VOLCANO_C_BEGIN extern "C" {
#   define VOLCANO_C_END }
#else
#   define VOLCANO_C
#   define VOLCANO_C_BEGIN
#   define VOLCANO_C_END
#endif

#if defined(__GNUC__)
#   define VOLCANO_FORCE_INLINE __inline __attribute__((__always_inline__))
#elif defined(_MSC_VER)
#   define VOLCANO_FORCE_INLINE __forceinline
#else
#   error Unknown compiler.
#endif

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using namespace std::chrono_literals;

using Buffer = std::vector<uint8_t>;
using StringList = std::list<std::string>;
using StringVector = std::vector<std::string>;

using Clock = std::chrono::steady_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;

VOLCANO_FORCE_INLINE int64_t durationToSeconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::seconds>(d).count();
}

VOLCANO_FORCE_INLINE int64_t durationToMilliseconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
}

VOLCANO_FORCE_INLINE int64_t durationToMicroseconds(Duration d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

VOLCANO_FORCE_INLINE int64_t timePointToSeconds(TimePoint tp) {
    return durationToSeconds(tp.time_since_epoch());
}

VOLCANO_FORCE_INLINE int64_t timePointToMillieconds(TimePoint tp) {
    return durationToMilliseconds(tp.time_since_epoch());
}

VOLCANO_FORCE_INLINE int64_t timePointToMicroseconds(TimePoint tp) {
    return durationToMicroseconds(tp.time_since_epoch());
}

template <typename... Args>
void log(SDL_LogPriority priority, std::format_string<Args...> fmt, Args&&... args) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, std::format(fmt, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
void logError(std::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_ERROR, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logWarning(std::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_WARN, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logInfo(std::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_INFO, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logDebug(std::format_string<Args...> fmt, Args&&... args) {
#ifdef VOLCANO_DEBUG
    log(SDL_LOG_PRIORITY_DEBUG, fmt, std::forward<Args>(args)...);
#endif
}

VOLCANO_END

#endif // VOLCANO_COMMON_H
