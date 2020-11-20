//
//
#ifndef VOLCANO_APPLICATION_HPP
#define VOLCANO_APPLICATION_HPP

#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Volcano/Looper/Common.hpp>

VOLCANO_LOOPER_BEGIN

class Handle {
public:
    enum Priority {
        PRIORITY_HIGH = 0,
        PRIORITY_NORMAL,
        PRIORITY_LOW,
        PRIORITY_MAX
    };

public:
    Handle(Callback cb, Priority prio);
    virtual ~Handle(void);

public:
    static void init(void);
    static void shutdown(void);
    static void run(void);
    static void stop(void);

protected:
    void enable(void);
    void disable(void);
    bool isEnabled(void) const;
    void invokeCallback(void);
    virtual Duration update(TimePoint now) = 0;

private:
    using Handles = std::vector<Handle *>;
    static Duration updateAll(TimePoint now);
    static Duration updateHandles(Handles &c, TimePoint now);

private:
    std::thread::id m_threadId;
    Callback m_cb;
    Priority m_priority;
    bool m_enabled;

private:
    static std::mutex c_mutex;
    static std::condition_variable c_cond;
    static bool c_running;
    thread_local static Handles c_handles[PRIORITY_MAX];
};

VOLCANO_INLINE void Handle::stop(void)
{
    c_running = false;
}

VOLCANO_INLINE void Handle::enable(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_threadId);
    VOLCANO_ASSERT(!m_enabled);
    auto c = c_handles[m_priority];
    VOLCANO_ASSERT(std::find(c.begin(), c.end(), this) == c.end());
    c.push_back(this);
    m_enabled = true;
}

VOLCANO_INLINE void Handle::disable(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_threadId);
    VOLCANO_ASSERT(m_enabled);
    auto c = c_handles[m_priority];
    auto it(std::find(c.begin(), c.end(), this));
    VOLCANO_ASSERT(it != c.end());
    *it = nullptr;
    m_enabled = false;
}

VOLCANO_INLINE bool Handle::isEnabled(void) const
{
    return m_enabled;
}

VOLCANO_INLINE void Handle::invokeCallback(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_threadId);

    m_cb();
}

VOLCANO_LOOPER_END

#endif // VOLCANO_APPLICATION_HPP
