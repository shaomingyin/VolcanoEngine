//
//
#include <Volcano/Common.h>

extern "C" uv_loop_t *uv_current_loop(void)
{
    thread_local static uv_loop_t loop;
    thread_local static uv_loop_t *p = nullptr;

    if (p == nullptr) {
        uv_loop_init(&loop);
        p = &loop;
    }

    return p;
}
