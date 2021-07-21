//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <QtGlobal>
#include <QtConcurrent>

#include <QDebug>

#include <Volcano/Config.hpp>

#if defined(_MSC_VER)
#   define VOLCANO_INLINE __forceinline
#elif defined(__GNUC__)
#   define VOLCANO_INLINE inline __attribute__((always_inline))
#else
#    error unsupported compiler.
#endif

#define VOLCANO_CONCAT(a, b) VOLCANO_CONCAT_IMPL(a, b)
#define VOLCANO_CONCAT_IMPL(a, b) a##b

#define VOLCANO_CONCAT3(a, b, c) VOLCANO_CONCAT3_IMPL(a, b, c)
#define VOLCANO_CONCAT3_IMPL(a, b, c) a##b##c

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_IMPL(x)
#define VOLCANO_STRIZE_IMPL(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#endif // VOLCANO_COMMON_HPP
