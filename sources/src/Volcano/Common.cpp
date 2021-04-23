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

void uvSyncClose(uv_handle_t *p)
{
    VOLCANO_ASSERT(p != nullptr);

    bool closed = false;
    p->data = &closed;

    uv_close(p, [] (uv_handle_t *handle) {
        *reinterpret_cast<bool *>(handle->data) = true;
    });

    while (!closed)
        uv_run(p->loop, UV_RUN_ONCE);
}

VOLCANO_END
