//
//
#include <unordered_map>

#include <Volcano/Looper.h>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

// ObjectContext

class ObjectContext;
using ObjectContextMap = std::unordered_map<std::thread::id, ObjectContext *>;

static ObjectContextMap objectContextMap;
static std::mutex objectContextMapMutex;
static std::condition_variable objectContextMapCond;

class ObjectContext final {
public:
    ObjectContext(void);
    ~ObjectContext(void);

public:
    std::thread::id threadId(void) const;
    VolcanoLooperContext *looper(void);
    Object *rootObject(void);
    const SDL_Event *nextEvent(void);
    void schedule(void);
    static void postKick(std::thread::id threadId);
    static void postQuit(std::thread::id threadId);
    static void postEvent(std::thread::id threadId, const SDL_Event &evt);

private:
    static void kickAsync(VolcanoLooperAsync *handle);
    static void quitAsync(VolcanoLooperAsync *handle);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    std::thread::id m_threadId;
    VolcanoLooperContext m_looper;
    VolcanoLooperAsync m_kickAsync;
    VolcanoLooperAsync m_quitAsync;
    Object m_rootObject;
    SDL_Event m_eventQueue[EVENT_QUEUE_SIZE];
    int64_t m_eventFirst;
    int64_t m_eventLast;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};

ObjectContext::ObjectContext(void):
    m_threadId(std::this_thread::get_id()),
    m_eventFirst(0),
    m_eventLast(0)
{
    volcanoLooperContextInit(&m_looper);

    volcanoLooperAsyncInit(&m_looper, &m_kickAsync, &ObjectContext::kickAsync);
    m_kickAsync.base.userData = this;

    volcanoLooperAsyncInit(&m_looper, &m_quitAsync, &ObjectContext::quitAsync);
    m_quitAsync.base.userData = this;

    {
        std::unique_lock locker(objectContextMapMutex);
        VOLCANO_ASSERT(objectContextMap.count(m_threadId) == 0);
        objectContextMap[m_threadId] = this;
    }
}

ObjectContext::~ObjectContext(void)
{
    VOLCANO_ASSERT(m_threadId == std::this_thread::get_id());

    {
        std::unique_lock locker(objectContextMapMutex);
        VOLCANO_ASSERT(objectContextMap.count(m_threadId) == 1);
        objectContextMap.erase(m_threadId);
    }

    volcanoLooperHandleClose(&m_quitAsync.base);
    volcanoLooperHandleClose(&m_kickAsync.base);

    volcanoLooperContextDestroy(&m_looper);
}

VOLCANO_INLINE std::thread::id ObjectContext::threadId(void) const
{
    return m_threadId;
}

VOLCANO_INLINE VolcanoLooperContext *ObjectContext::looper(void)
{
    return &m_looper;
}

VOLCANO_INLINE Object *ObjectContext::rootObject(void)
{
    return &m_rootObject;
}

VOLCANO_INLINE const SDL_Event *ObjectContext::nextEvent(void)
{
    if (m_eventFirst < m_eventLast)
        return &m_eventQueue[m_eventFirst++];
    return nullptr;
}

VOLCANO_INLINE void ObjectContext::postKick(std::thread::id threadId)
{
    VOLCANO_ASSERT(threadId != std::this_thread::get_id());

    std::unique_lock locker(objectContextMapMutex);
    auto it(objectContextMap.find(threadId));
    if (it != objectContextMap.end()) {
        auto p = it->second;
        volcanoLooperAsyncSend(&p->m_kickAsync);
    }
}

VOLCANO_INLINE void ObjectContext::postQuit(std::thread::id threadId)
{
    VOLCANO_ASSERT(threadId != std::this_thread::get_id());

    std::unique_lock locker(objectContextMapMutex);
    auto it(objectContextMap.find(threadId));
    if (it != objectContextMap.end()) {
        auto p = it->second;
        volcanoLooperAsyncSend(&p->m_quitAsync);
    }
}

VOLCANO_INLINE void ObjectContext::postEvent(std::thread::id threadId, const SDL_Event &evt)
{
    std::unique_lock locker(objectContextMapMutex);
    auto it(objectContextMap.find(threadId));
    if (it != objectContextMap.end()) {
        auto p = it->second;
        p->m_eventQueue[p->m_eventLast++] = evt;
    }
}

void ObjectContext::quitAsync(VolcanoLooperAsync *p)
{
    auto ctx = reinterpret_cast<ObjectContext *>(p->base.userData);
    volcanoLooperContextStop(&ctx->m_looper);
}

void ObjectContext::kickAsync(VolcanoLooperAsync *p)
{
    VOLCANO_UNUSED(p);
}

// Object

thread_local static ObjectContext current;

Uint32 Object::c_last;
int Object::c_frameCount;
int Object::c_frameCountPerSecond;

Object::Object(Object *parent):
    m_looper(current.looper()),
    m_parent(nullptr),
    m_childrenCache(32),
    m_enabled(false)
{
    setParent(parent);
}

Object::~Object(void)
{
    if (isEnabled())
        disable();

    setParent(nullptr);
}

Object *Object::parent(void)
{
    if (m_parent != current.rootObject())
        return m_parent;

    return nullptr;
}

void Object::setParent(Object *p)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_threadId);

    if (m_parent != nullptr) {
        m_parent->m_children.remove(this);
        m_parent->onChildRemoved(this);
    }

#ifdef VOLCANO_DEBUG
    if (p != nullptr)
        VOLCANO_ASSERT(m_threadId == p->m_threadId);
#endif

    m_parent = p;

    if (m_parent == nullptr && this != current.rootObject())
        m_parent = current.rootObject();

    if (m_parent != nullptr) {
        m_parent->m_children.push_back(this);
        m_parent->onChildAdded(this);
    }
}

void Object::enable(void)
{
    if (!m_enabled) {
        m_enabled = true;
        onEnabled();
    }
}

void Object::disable(void)
{
    if (m_enabled) {
        m_enabled = false;
        onDisabled();
    }
}

void Object::run(int fps)
{
    VolcanoLooperTimer frameTimer;
    volcanoLooperTimerInit(current.looper(), &frameTimer);
    volcanoLooperTimerStart(&frameTimer, &Object::frame, 0,
        std::chrono::milliseconds(std::chrono::seconds(1) / fps).count());

    VolcanoLooperTimer frameCountTimer;
    volcanoLooperTimerInit(current.looper(), &frameCountTimer);
    volcanoLooperTimerStart(&frameCountTimer, &Object::frameCount, 1000, 1000);

    c_frameCount = 0;
    c_frameCountPerSecond = 0;
    c_last = current.looper()->now;

    volcanoLooperContextRun(current.looper());

    volcanoLooperTimerStop(&frameCountTimer);
    volcanoLooperTimerStop(&frameTimer);
}

void Object::quit(void)
{
    volcanoLooperContextStop(current.looper());
}

void Object::postQuit(std::thread::id threadId)
{
    ObjectContext::postQuit(threadId);
}

void Object::kick(std::thread::id threadId)
{
    ObjectContext::postKick(threadId);
}

void Object::postEvent(std::thread::id threadId, const SDL_Event &evt)
{
    ObjectContext::postEvent(threadId, evt);
}

void Object::updateChildrenCache(void)
{
    m_childrenCache.clear();

    for (auto child: m_children) {
        m_childrenCache.push_back(child);
        child->updateChildrenCache();
    }
}

void Object::update(VolcanoLooperPrepare *p)
{
    VOLCANO_UNUSED(p);

    Uint32 now = current.looper()->now;
    Uint32 pass = now - c_last;

    for (auto evt = current.nextEvent(); evt != nullptr; evt = current.nextEvent())
        current.rootObject()->onEvent(*evt);

    current.rootObject()->onTick(std::chrono::milliseconds(pass));

    c_last = now;
    c_frameCount += 1;
}

void Object::onChildAdded(Object *child)
{
    VOLCANO_UNUSED(child);
}

void Object::onChildRemoved(Object *child)
{
    VOLCANO_UNUSED(child);
}

void Object::onNameChanged(const std::string &name)
{
    VOLCANO_UNUSED(name);
}

void Object::onEnabled(void)
{
}

void Object::onDisabled(void)
{
}

void Object::onTick(Duration elapsed)
{
    if (m_enabled || this == current.rootObject()) {
        for (auto child: m_childrenCache)
            child->onTick(elapsed);
    }
}

bool Object::onEvent(const SDL_Event &evt)
{
    if (m_enabled || this == current.rootObject()) {
        for (auto child: m_childrenCache) {
            if (child->onEvent(evt))
                return true;
        }
    }

    return false;
}

VOLCANO_END
