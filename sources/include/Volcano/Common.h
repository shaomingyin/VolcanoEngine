/*
 *
 */
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <Volcano/Config.h>

#ifdef __cplusplus
#   define VOLCANO_C_BEGIN extern "C" {
#   define VOLCANO_C_END }
#else
#   define VOLCANO_C_BEGIN
#   define VOLCANO_C_END
#endif

#ifdef VOLCANO_DEBUG
#   define SDL_ASSERT_LEVEL 3
#   include <SDL_assert.h>
#   define VOLCANO_ASSERT(expr) SDL_assert(expr)
#else
#   define VOLCANO_ASSERT(expr) do { } while (0)
#endif

#include <physfs.h>
#include <SDL.h>
#include <uv.h>

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

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_C_BEGIN

void volcanoMemcpyReverse(void *dst, const void *src, size_t len);
void volcanoLog(SDL_LogPriority prio, const char *fmt, ...);

VOLCANO_C_END

#define VOLCANO_LOGI(fmt, ...) volcanoLog(SDL_LOG_PRIORITY_INFO, fmt, __VA_ARGS__)
#define VOLCANO_LOGW(fmt, ...) volcanoLog(SDL_LOG_PRIORITY_WARN, fmt, __VA_ARGS__)
#define VOLCANO_LOGE(fmt, ...) volcanoLog(SDL_LOG_PRIORITY_ERROR, fmt, __VA_ARGS__)

#ifdef VOLCANO_DEBUG
#   define VOLCANO_LOGD(fmt, ...) volcanoLog(SDL_LOG_PRIORITY_DEBUG, fmt, __VA_ARGS__)
#else
#   define VOLCANO_LOGD(fmt, ...) do { } while (0)
#endif

#endif // VOLCANO_COMMON_HPP
