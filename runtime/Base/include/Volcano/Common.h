//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <cstdlib>
#include <cstdint>
#include <cstdarg>

#include <set>
#include <list>
#include <vector>
#include <string>
#include <chrono>

#include <SDL.h>
#include <fmt/core.h>
#include <sigslot/signal.hpp>

#include <Volcano/Config.h>
#include <Volcano/Eigen.h>

#ifdef VOLCANO_PROFILE
#   include <microprofile.h>
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

#ifdef VOLCANO_DEBUG
#   include <SDL_assert.h>
#   define VOLCANO_ASSERT(expr) SDL_assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#if defined(__GNUC__)
#   if __GNUC__ >= 4
#       define VOLCANO_EXPORT __attribute__((visibility("default")))
#   else
#       define VOLCANO_EXPORT
#   endif
#   define VOLCANO_IMPORT
#   define VOLCANO_FORCE_INLINE __inline __attribute__((__always_inline__))
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

#ifdef VolcanoBase_SHARED
#   ifdef VolcanoBase_EXPORTS
#       define VOLCANO_BASE_API VOLCANO_EXPORT
#   else
#       define VOLCANO_BASE_API VOLCANO_IMPORT
#   endif
#else
#   define VOLCANO_BASE_API
#endif

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

#define VOLCANO_PMOVB(p, offset) \
    (((uint8_t*)p) + (offset))

#define VOLCANO_DIMOF(a) (sizeof(a) / sizeof(a[0]))

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using namespace std::chrono_literals;

using Buffer = std::vector<uint8_t>;
using StringSet = std::set<std::string>;
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
void log(SDL_LogPriority priority, fmt::format_string<Args...> fmt, Args&&... args) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority, fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
void logError(fmt::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_ERROR, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logWarning(fmt::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_WARN, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logInfo(fmt::format_string<Args...> fmt, Args&&... args) {
    log(SDL_LOG_PRIORITY_INFO, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logDebug(fmt::format_string<Args...> fmt, Args&&... args) {
#ifdef VOLCANO_DEBUG
    log(SDL_LOG_PRIORITY_DEBUG, fmt, std::forward<Args>(args)...);
#endif
}

VOLCANO_END

#endif // VOLCANO_COMMON_H
