//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <QtGlobal>
#include <QtConcurrent>
#include <QtLogging>

#include <QVersionNumber>

#include <Volcano/Config.h>

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

extern const QVersionNumber version;

VOLCANO_FORCE_INLINE QDebug fatal() {
    return qFatal().noquote().nospace();
}

VOLCANO_FORCE_INLINE QDebug error() {
    return qCritical().noquote().nospace();
}

VOLCANO_FORCE_INLINE QDebug warning() {
    return qWarning().noquote().nospace();
}

VOLCANO_FORCE_INLINE QDebug info() {
    return qInfo().noquote().nospace();
}

VOLCANO_FORCE_INLINE QDebug debug() {
    return qDebug().noquote().nospace();
}

VOLCANO_END

#endif // VOLCANO_COMMON_H
