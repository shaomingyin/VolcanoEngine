/*
 *
 */
#ifndef CX_COMMON_H
#define CX_COMMON_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <cx/config.h>

#ifdef __cplusplus
#   define CX_BEGIN_DECLS extern "C" {
#   define CX_END_DECLS }
#else
#   define CX_BEGIN_DECLS
#   define CX_END_DECLS
#endif

#ifdef CX_DEBUG
#   include <assert.h>
#   define CX_ASSERT(expr) assert(expr)
#else
#   define CX_ASSERT(expr) do { } while (0)
#endif

#if defined(_MSC_VER)
#   define CX_FORCE_INLINE __forceinline
#   define CX_LIKELY(x) x
#   define CX_UNLIKELY(x) x
#   define CX_THREAD __declspec(thread)
#   define CX_DEPRECATED(msg) __declspec(deprecated(msg))
#elif defined(__GNUC__) || defined(__clang__)
#   define CX_FORCE_INLINE inline __attribute__((always_inline))
#   define CX_LIKELY(x) x
#   define CX_UNLIKELY(x) x
#   define CX_THREAD __thread
#   define CX_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#    error unsupported compiler.
#endif

#ifndef CX_API
#   define CX_API
#endif

#define CX_UNUSED(param) ((void)(param))

#define CX_CONCAT(a, b) CX_CONCAT_IMPL(a, b)
#define CX_CONCAT_IMPL(a, b) a##b

#define CX_CONCAT3(a, b, c) CX_CONCAT_IMPL(a, b, c)
#define CX_CONCAT3_IMPL(a, b, c) a##b##c

#define CX_STRIZE(x) CX_STRIZE_IMPL(x)
#define CX_STRIZE_IMPL(x) #x

#ifdef __COUNTER__
#   define CX_AUTONAME(str) CX_CONCAT(str, __COUNTER__)
#else
#   define CX_AUTONAME(str) CX_CONCAT(str, __LINE__)
#endif

#define CX_PMOVB(p, offset) \
    (((unsigned char *)(p)) + (offset))

#define CX_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type *)0)->member_name)))

#define CX_MEMBEROF(p, struct_type, member_name) \
    ((struct_type *)CX_PMOVB(p, -CX_OFFSETOF(struct_type, member_name)))

CX_BEGIN_DECLS

CX_API void cx_memrcpy(void *dst, const void *src, size_t len);

CX_END_DECLS

#endif // CX_COMMON_HPP
