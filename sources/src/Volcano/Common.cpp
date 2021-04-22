//
//
#include <cstdarg>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

void writeLog(SDL_LogPriority prio, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, prio, fmt, ap);
    va_end(ap);
}

VOLCANO_END
