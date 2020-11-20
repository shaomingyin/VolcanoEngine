//
//
#ifndef VOLCANO_OBJECT_HPP
#define VOLCANO_OBJECT_HPP

#include <list>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Object;
using ObjectList = std::list<Object *>;

class Object {
public:
    Object(Object *parent = nullptr);
    virtual ~Object(void);

public:
    Object *parent(void);
    void setParent(Object *p);
    const ObjectList children(void) const;
    const std::string &name(void) const;
    void setName(const std::string &name);
    bool isEnabled(void) const;
    void enable(void);
    void disable(void);
    std::thread::id threadId(void) const;
    VolcanoLooperContext *looper(void);
    static void run(int fps = 60);
    static void quit(void);
    static void kick(std::thread::id threadId);
    static void kick(std::thread *thread);
    static void postQuit(std::thread::id threadId);
    static void postQuit(std::thread *thread);
    static void postEvent(std::thread::id threadId, const SDL_Event &evt);
    static void postEvent(std::thread *thread, const SDL_Event &evt);
    TimePoint now(void);

protected:
    void updateChildrenCache(void);
    static void update(VolcanoLooperPrepare *p);
    virtual void onChildAdded(Object *child);
    virtual void onChildRemoved(Object *child);
    virtual void onNameChanged(const std::string &name);
    virtual void onEnabled(void);
    virtual void onDisabled(void);
    virtual void onTick(Duration elapsed);
    virtual bool onEvent(const SDL_Event &evt);

private:
    using ObjectVector = std::vector<Object *>;

private:
    std::thread::id m_threadId;
    VolcanoLooperContext *m_looper;
    Object *m_parent;
    ObjectList m_children;
    ObjectVector m_childrenCache;
    std::string m_name;
    bool m_enabled;
    static Uint32 c_last;
    static int c_frameCount;
    static int c_frameCountPerSecond;
};

VOLCANO_INLINE const ObjectList Object::children(void) const
{
    return m_children;
}

VOLCANO_INLINE const std::string &Object::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void Object::setName(const std::string &name)
{
    if (m_name != name) {
        m_name = name;
        onNameChanged(name);
    }
}

VOLCANO_INLINE bool Object::isEnabled(void) const
{
    return m_enabled;
}

VOLCANO_INLINE std::thread::id Object::threadId(void) const
{
    return m_threadId;
}

VOLCANO_INLINE uv_loop_t *Object::loop(void)
{
    return m_loop;
}

VOLCANO_INLINE void Object::kick(std::thread *thread)
{
    VOLCANO_ASSERT(thread != nullptr);

    kick(thread->get_id());
}

VOLCANO_INLINE void Object::postQuit(std::thread *thread)
{
    VOLCANO_ASSERT(thread != nullptr);

    postQuit(thread->get_id());
}

VOLCANO_INLINE void Object::postEvent(std::thread *thread, const SDL_Event &evt)
{
    VOLCANO_ASSERT(thread != nullptr);

    postEvent(thread->get_id(), evt);
}

VOLCANO_INLINE TimePoint Object::now(void)
{
    return c_now;
}

VOLCANO_END

#endif // VOLCANO_OBJECT_HPP
