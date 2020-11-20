//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <cstdint>
#include <list>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>

#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Volcano/Common.h>
#include <Volcano/Looper.h>

#ifdef max
#    undef max
#endif

#ifdef min
#    undef min
#endif

#include <rttr/type>
#include <rttr/registration>

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

using ByteArray = std::vector<uint8_t> ;
using StringList = std::list<std::string>;
using Clock = std::chrono::high_resolution_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;
using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Callback = std::function<void (void)>;

class Noncopyable {
public:
    inline Noncopyable(void) { }
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};

static inline void uvSyncClose(uv_handle_t *p)
{
    int done = 0;
    p->data = &done;
    uv_close(p, [](uv_handle_t *p) { *reinterpret_cast<int *>(p->data) = 1; });
    while (!done) { uv_run(p->loop, UV_RUN_ONCE); }
}

static inline void uvSyncClose(uv_timer_t *p)
{
    uvSyncClose(reinterpret_cast<uv_handle_t *>(p));
}

static inline void uvSyncClose(uv_async_t *p)
{
    uvSyncClose(reinterpret_cast<uv_handle_t *>(p));
}

static inline void uvSyncClose(uv_prepare_t *p)
{
    uvSyncClose(reinterpret_cast<uv_handle_t *>(p));
}

static inline bool isWritablePath(const std::string &path)
{
    static const std::filesystem::perms write_perms =
        std::filesystem::perms::owner_write |
        std::filesystem::perms::group_write |
        std::filesystem::perms::others_write;

    std::filesystem::perms perms = std::filesystem::status(path).permissions();

    return ((perms & write_perms) != std::filesystem::perms::none);
}

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
