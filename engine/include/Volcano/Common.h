/*
 *
 */
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <stdbool.h>

#include <Volcano/Config.h>

#ifdef VOLCANO_DEBUG
#    define SDL_ASSERT_LEVEL 2
#    include <SDL_assert.h>
#    define VOLCANO_ASSERT(expr) SDL_assert(expr)
#else
#    define VOLCANO_ASSERT(expr) do { } while (0)
#endif

#include <uv.h>
#include <SDL.h>

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

#define VOLCANO_PMOVB(p, offset) \
    (((unsigned char *)(p)) + (offset))

#define VOLCANO_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type *)0)->member_name)))

#define VOLCANO_MEMBEROF(p, struct_type, member_name) \
    ((struct_type *)VOLCANO_PMOVB(p, -VOLCANO_OFFSETOF(struct_type, member_name)))

#define VOLCANO_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION

#define VOLCANO_LOGE(fmt, ...) SDL_LogMessage(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_ERROR, fmt, ##__VA_ARGS__)
#define VOLCANO_LOGW(fmt, ...) SDL_LogMessage(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)
#define VOLCANO_LOGI(fmt, ...) SDL_LogMessage(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)

#ifdef VOLCANO_DEBUG
#    define VOLCANO_LOGD(fmt, ...) SDL_LogMessage(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)
#else
#    define VOLCANO_LOGD(fmt, ...)
#endif

VOLCANO_BEGIN_C_DECLS

uv_loop_t *uv_current_loop(void);

VOLCANO_END_C_DECLS

#endif /* VOLCANO_H */
