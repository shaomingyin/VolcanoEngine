/*
 *
 */
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <stdbool.h>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#   ifndef VOLCANO_ASSERT
#       include <assert.h>
#       define VOLCANO_ASSERT(expr) assert(expr)
#   endif
#else
#    define VOLCANO_ASSERT(expr) do { } while (0)
#endif

#ifdef __cplusplus
#    define VOLCANO_BEGIN_C_DECLS extern "C" {
#    define VOLCANO_END_C_DECLS }
#else
#    define VOLCANO_BEGIN_C_DECLS
#    define VOLCANO_END_C_DECLS
#endif

#if defined(_MSC_VER)
#    define VOLCANO_LIKELY(x) (x)
#    define VOLCANO_UNLIKELY(x) (x)
#    define VOLCANO_FORCE_INLINE __forceinline
#    define VOLCANO_PRINTF_LIKE(a, b)
#    define VOLCANO_INT64 __int64
#    define VOLCANO_UINT64 unsigned __int64
#elif defined(__GNUC__)
#    define VOLCANO_LIKELY(x) __builtin_expect(!!(x), 1)
#    define VOLCANO_UNLIKELY(x) __builtin_expect(!!(x), 0)
#    define VOLCANO_FORCE_INLINE inline __attribute__((always_inline))
#    define VOLCANO_PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#    ifdef _LP64
#        define VOLCANO_INT64 long
#        define VOLCANO_UINT64 unsigned long
#    else
#        define VOLCANO_INT64 long long
#        define VOLCANO_UINT64 unsgined long long
#    endif
#else
#    error unsupported compiler.
#endif

#if defined(_WIN32)
#    define VOLCANO_OS_WINDOWS
#elif defined(__linux__)
#    define VOLCANO_OS_LINUX
#else
#    error unknown os.
#endif

#define VOLCANO_UNUSED(x) ((void)(x))

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

#define VOLCANO_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION

VOLCANO_BEGIN_C_DECLS

VOLCANO_END_C_DECLS

#endif /* VOLCANO_COMMON_H */
