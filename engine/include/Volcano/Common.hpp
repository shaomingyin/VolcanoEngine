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

#ifdef max
#    undef max
#endif

#ifdef min
#    undef min
#endif

#include <rttr/type>
#include <rttr/registration>

#include <sigslot/signal.hpp>

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

template <typename... T>
using signal = sigslot::signal_st<T...>;

static inline void uv_close_sync(uv_async_t *async)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(async));
}

static inline void uv_close_sync(uv_prepare_t *prepare)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(prepare));
}

static inline void uv_close_sync(uv_timer_t *timer)
{
    uv_close_sync(reinterpret_cast<uv_handle_t *>(timer));
}

template <typename FN>
class ScopedExit final {
public:
    ScopedExit(FN fn):
        m_fn(fn)
    {
    }

    ~ScopedExit(void)
    {
        if (m_fn)
            m_fn();
    }

private:
    FN m_fn;
};

class Noncopyable {
public:
    inline Noncopyable(void) { }
    Noncopyable(const Noncopyable &) = delete;
    Noncopyable &operator=(const Noncopyable &) = delete;
};

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
