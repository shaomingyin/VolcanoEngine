//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <list>
#include <vector>
#include <string>
#include <cassert>
#include <cstdint>

#include <rttr/type>
#include <rttr/registration>

#include <sigslot/signal.hpp>

#include <cx/common.h>

#include <Volcano/Config.hpp>

#ifdef VOLCANO_DEBUG
#   ifdef EIGEN_NO_DEBUG
#       undef EIGEN_NO_DEBUG
#   endif
#   define SDL_ASSERT_LEVEL 3
#   include <SDL_assert.h>
#   define VOLCANO_ASSERT(expr) SDL_assert(expr)
#else
#   ifndef EIGEN_NO_DEBUG
#       define EIGEN_NO_DEBUG
#   endif
#   define VOLCANO_ASSERT(expr) do { } while (0)
#endif

#define VOLCANO_INLINE CX_FORCE_INLINE

#include <SDL.h>
#include <uv.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

enum class ByteOrder {
    BigEndian = 0,
    LittleEndian
};

using ByteArray = std::vector<uint8_t>;
using StringList = std::list<std::string>;

void writeLog(SDL_LogPriority prio, const char *fmt, ...);

VOLCANO_END

#define VOLCANO_LOGE(fmt, ...) Volcano::writeLog(SDL_LOG_PRIORITY_ERROR, fmt, __VA_ARGS__)
#define VOLCANO_LOGW(fmt, ...) Volcano::writeLog(SDL_LOG_PRIORITY_WARN, fmt, __VA_ARGS__)
#define VOLCANO_LOGI(fmt, ...) Volcano::writeLog(SDL_LOG_PRIORITY_INFO, fmt, __VA_ARGS__)

#ifdef VOLCANO_DEBUG
#   define VOLCANO_LOGD(fmt, ...) Volcano::writeLog(SDL_LOG_PRIORITY_DEBUG, fmt, __VA_ARGS__)
#else
#   define VOLCANO_LOGD(fmt, ...) do { } while (0)
#endif

#endif // VOLCANO_COMMON_HPP
