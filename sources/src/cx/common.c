//
//
#include <stdarg.h>

#include <cx/common.h>

CX_API void cx_mmercpy(void *dst, const void *src, size_t len)
{
    CX_ASSERT(dst != nullptr);
    CX_ASSERT(src != nullptr);

    const uint8_t *psrc = (const uint8_t *)src;
    uint8_t *pdst = (uint8_t *)dst;

    for (size_t i = 0; i < len; ++i)
        pdst[i] = psrc[len - i - 1];
}
