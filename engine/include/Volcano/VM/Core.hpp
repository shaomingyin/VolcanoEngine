//
//
#ifndef VOLCANO_VM_CORE_HPP
#define VOLCANO_VM_CORE_HPP

#include <string>
#include <thread>
#include <future>

#include <Volcano/SpinLock.hpp>
#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Window.hpp>
#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/RootFileSystem.hpp>
#include <Volcano/VM/World.hpp>

VOLCANO_VM_BEGIN

class Core: public Noncopyable {
public:
    Core(uv_loop_t *loop);
    virtual ~Core(void);

public:
    virtual bool start(const std::string &rootPath, const std::string &initPath);
    virtual void stop(void);
    virtual void postEvent(const SDL_Event &evt);
    uv_loop_t *loop(void);
    const std::thread &thread(void) const;
    World &world(void);

protected:
    // Called in main thread.
    void kick(void);

    // Called in vm thread.
    virtual void run(uv_loop_t *loop, std::promise<bool> *initPromise);

private:
    void main(std::promise<bool> *initResult);
    void frame(float elapsed);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    uv_loop_t *m_loop;
    bool m_started;
    uv_async_t m_renderAsync;
    Graphics::Memory m_graphicsMemory;
    Graphics::Renderer m_graphicsRenderer;
    Graphics::Window m_graphicsWindow;
    Graphics::View m_viewFlip[2];
    int m_viewRendering;
    SpinLock m_viewLock;
    std::thread m_thread;
    uv_async_t m_quitAsync;
    uv_async_t m_kickAsync;
    std::string m_rootPath;
    std::string m_initPath;
    SDL_Event m_eventQueue[EVENT_QUEUE_SIZE];
    uint64_t m_eventFirst;
    uint64_t m_eventLast;
    RootFileSystem m_fs;
    World m_world;
    int64_t m_lastFrameTime;
};

VOLCANO_INLINE uv_loop_t *Core::loop(void)
{
    return m_loop;
}

VOLCANO_INLINE const std::thread &Core::thread(void) const
{
    return m_thread;
}

VOLCANO_INLINE World &Core::world(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_thread.get_id());

    return m_world;
}

VOLCANO_INLINE void Core::kick(void)
{
    VOLCANO_ASSERT(m_started);
    VOLCANO_ASSERT(std::this_thread::get_id() != m_thread.get_id());

    uv_async_send(&m_kickAsync);
}

VOLCANO_VM_END

#endif // VOLCANO_VM_CORE_HPP
