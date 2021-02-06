//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <QtGlobal>
#include <QDebug>

#include <Volcano/Config.hpp>

#if defined(_MSC_VER)
#    define VOLCANO_FORCE_INLINE __forceinline
#elif defined(__GNUC__)
#    define VOLCANO_FORCE_INLINE inline __attribute__((always_inline))
#else
#    error unsupported compiler.
#endif

#define VOLCANO_INLINE VOLCANO_FORCE_INLINE

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

#ifdef VOLCANO
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API //Q_DECL_IMPORT
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

#endif // VOLCANO_COMMON_HPP
