//
//
#include <cstdarg>

#include <Volcano/Common.hpp>

extern "C" void volcanoMemcpyReverse(void *dst, const void *src, size_t len)
{
    VOLCANO_ASSERT(dst != nullptr);
    VOLCANO_ASSERT(src != nullptr);

    auto *psrc = reinterpret_cast<const uint8_t *>(src);
    auto *pdst = reinterpret_cast<uint8_t *>(dst);

    for (size_t i = 0; i < len; ++i)
        pdst[i] = psrc[len - i - 1];
}

extern "C" void volcanoLog(SDL_LogPriority prio, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, prio, fmt, ap);
    va_end(ap);
}
