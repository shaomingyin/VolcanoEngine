//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <list>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <cstdarg>
#include <cstdint>
#include <cstdlib>

#include <Volcano/Config.hpp>

#ifdef VOLCANO_DEBUG
#   include <cassert>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr) do { } while (0)
#endif

#if defined(_MSC_VER)
#   define VOLCANO_INLINE __forceinline
#   define VOLCANO_LIKELY(x) x
#   define VOLCANO_UNLIKELY(x) x
#elif defined(__GNUC__)
#   define VOLCANO_INLINE inline __attribute__((always_inline))
#   define VOLCANO_LIKELY(x) x
#   define VOLCANO_UNLIKELY(x) x
#else
#    error unsupported compiler.
#endif

#define VOLCANO_UNUSED(param) ((void)(param))

#define VOLCANO_CONCAT(a, b) VOLCANO_CONCAT_IMPL(a, b)
#define VOLCANO_CONCAT_IMPL(a, b) a##b

#define VOLCANO_CONCAT3(a, b, c) VOLCANO_CONCAT_IMPL(a, b, c)
#define VOLCANO_CONCAT3_IMPL(a, b, c) a##b##c

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_IMPL(x)
#define VOLCANO_STRIZE_IMPL(x) #x

#ifdef __COUNTER__
#   define VOLCANO_AUTONAME(str) VOLCANO_CONCAT(str, __COUNTER__)
#else
#   define VOLCANO_AUTONAME(str) VOLCANO_CONCAT(str, __LINE__)
#endif

#define VOLCANO_PMOVB(p, offset) \
    (((unsigned char *)(p)) + (offset))

#define VOLCANO_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type *)0)->member_name)))

#define VOLCANO_MEMBEROF(p, struct_type, member_name) \
    ((struct_type *)VOLCANO_PMOVB(p, -VOLCANO_OFFSETOF(struct_type, member_name)))

#ifdef VOLCANO_DEBUG
#   ifdef EIGEN_NO_DEBUG
#       undef EIGEN_NO_DEBUG
#   endif
#else
#   ifndef EIGEN_NO_DEBUG
#       define EIGEN_NO_DEBUG
#   endif
#endif

#include <spdlog/spdlog.h>
#include <sigslot/signal.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#define VOLCANO_DISABLE_COPY(className) \
    className(const className &) = delete; \
    className &operator=(const className &) = delete;

#define VOLCANO_DISABLE_MOVE(className) \
    className(className &&) = delete; \
    className &operator=(className &&) = delete;

#define VOLCANO_DISABLE_COPY_AND_MOVE(className) \
    VOLCANO_DISABLE_COPY(className) \
    VOLCANO_DISABLE_MOVE(className)

VOLCANO_BEGIN

enum class ByteOrder {
    BigEndian = 0,
    LittleEndian
};

using ByteArray = std::vector<uint8_t>;
using StringList = std::list<std::string>;

void makeLogger(std::shared_ptr<spdlog::logger> logger);

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
