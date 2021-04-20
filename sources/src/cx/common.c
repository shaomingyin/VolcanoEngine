//
//
#include <stdarg.h>

#include <cx/common.hpp>

void cx_mmercpy(void *dst, const void *src, size_t len)
{
    CX_ASSERT(dst != nullptr);
    CX_ASSERT(src != nullptr);

    auto *psrc = (const uint8_t *)sr);
    auto *pdst = (uint8_t *)dst;

    for (size_t i = 0; i < len; ++i)
        pdst[i] = psrc[len - i - 1];
}

#define CX_MATH_IMPLEMENTATION
#include <cx/math.h>
