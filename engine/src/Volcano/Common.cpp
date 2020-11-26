//
//
#include <Volcano/Common.h>

extern "C" void uv_close_sync(uv_handle_t *handle)
{
    uv_close(handle, [](uv_handle_t *handle) { uv_stop(handle->loop); });
    uv_run(handle->loop, UV_RUN_DEFAULT);
}
