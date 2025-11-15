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
#   include <assert.h>
#   define VOLCANO_ASSERT(expr) assert(expr)
#else
#   define VOLCANO_ASSERT(expr)
#endif

#define VOLCANO_PMOVB(p, offset) \
   (((uint8_t*)(p)) + (offset))

#define VOLCANO_OFFSETOF(struct_type, member_name) \
    ((size_t)&(((struct_type*)0)->member_name))

#define VOLCANO_MEMBEROF(p, struct_type, member_name) \
    ((struct_type*)VOLCANO_PMOVB(p, -VOLCANO_OFFSETOF(struct_type, member_name)))

#define VOLCANO_CON(a, b) VOLCANO_CON_(a, b)
#define VOLCANO_CON_(a, b) a##b

#define VOLCANO_CON3(a, b, c) VOLCANO_CON3_(a, b, c)
#define VOLCANO_CON3_(a, b, c) a##b##c

#define VOLCANO_STRIZE(x) VOLCANO_STRIZE_(x)
#define VOLCANO_STRIZE_(x) #x

#define VOLCANO_VERSION_STR \
    VOLCANO_STRIZE(VOLCANO_VERSION_MAJOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_MINOR) "." \
    VOLCANO_STRIZE(VOLCANO_VERSION_PATCH)

#endif // VOLCANO_COMMON_H
