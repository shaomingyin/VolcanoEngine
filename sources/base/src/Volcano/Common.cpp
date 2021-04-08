//
//
#include <Volcano/Common.hpp>

VOLCANO_BEGIN

void memcpyReverse(void *dst, const void *src, size_t len)
{
    VOLCANO_ASSERT(dst != nullptr);
    VOLCANO_ASSERT(src != nullptr);

    auto *psrc = reinterpret_cast<const uint8_t *>(src);
    auto *pdst = reinterpret_cast<uint8_t *>(dst);

    for (size_t i = 0; i < len; ++i)
        pdst[i] = psrc[len - i - 1];
}

VOLCANO_END
