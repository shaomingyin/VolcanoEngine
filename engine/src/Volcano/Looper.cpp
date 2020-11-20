//
//
#include <mutex>
#include <unordered_map>

#include <Volcano/Looper.hpp>

VOLCANO_LOOPER_BEGIN

class ThreadContext;
using ThreadContextMap = std::unordered_map<std::thread::id, ThreadContext *>;

static ThreadContextMap threadContextMap;
static std::mutex threadContextMapMutex;

class ThreadContext final {
public:
    ThreadContext(void);
    ~ThreadContext(void);

public:
    Context *context(void);

private:
    std::thread::id m_threadId;
    Context m_context;
    Context *m_pcontext;
};

ThreadContext::ThreadContext(void):
    m_threadId(std::this_thread::get_id())
{
    if (init(&m_context) == 0) {
        m_pcontext = &m_context;
        std::unique_lock locker(threadContextMapMutex);
        threadContextMap[m_threadId] = this;
    } else
        m_pcontext = nullptr;
}

ThreadContext::~ThreadContext(void)
{
    if (m_pcontext != nullptr) {
        std::unique_lock locker(threadContextMapMutex);
        threadContextMap.erase(m_threadId);
        destroy(&m_context);
        m_pcontext = nullptr;
    }
}

VOLCANO_INLINE Context *ThreadContext::context(void)
{
    return m_pcontext;
}

thread_local static ThreadContext threadContext;

Context *defaultContext(void)
{
    return threadContext.context();
}

void kickThread(std::thread::id threadId)
{
    VOLCANO_ASSERT(threadId != std::this_thread::get_id());

    std::unique_lock locker(threadContextMapMutex);

    auto it(threadContextMap.find(threadId));
    VOLCANO_ASSERT(it != threadContextMap.end());
    kick(it->second->context());
}

VOLCANO_LOOPER_END
