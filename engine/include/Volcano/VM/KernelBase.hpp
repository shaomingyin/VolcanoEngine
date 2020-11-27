//
//
#ifndef VOLCANO_VM_KERNELBASE_HPP
#define VOLCANO_VM_KERNELBASE_HPP

#include <string>
#include <thread>
#include <future>

#include <Volcano/SpinLock.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/RootFileSystem.hpp>
#include <Volcano/VM/World.hpp>

VOLCANO_VM_BEGIN

class KernelBase: public Noncopyable {
public:
    KernelBase(uv_loop_t *loop);
    virtual ~KernelBase(void);

public:
    bool start(const std::string &rootPath, const std::string &initPath);
    void stop(void);
    virtual void postEvent(const SDL_Event &evt);
    uv_loop_t *loop(void);
    const std::thread &thread(void) const;
    World &world(void);

protected:
    void kick(void);

    // Called in main thread.
    virtual bool init(void);
    virtual void shutdown(void);
    virtual Mesh *createMesh(void) = 0;

    // Called in vm thread.
    virtual void run(uv_loop_t *loop);
    virtual void frame(float elapsed);
    virtual void handleEvent(const SDL_Event &evt);

private:
    void threadMain(std::promise<bool> *initResult);

private:
    static const int EVENT_QUEUE_ORDER = 6;
    static const int EVENT_QUEUE_SIZE = 1 << EVENT_QUEUE_ORDER;
    static const int EVENT_QUEUE_MASK = EVENT_QUEUE_SIZE - 1;

private:
    uv_loop_t *m_loop;
    bool m_started;
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

VOLCANO_INLINE uv_loop_t *KernelBase::loop(void)
{
    return m_loop;
}

VOLCANO_INLINE const std::thread &KernelBase::thread(void) const
{
    return m_thread;
}

VOLCANO_INLINE World &KernelBase::world(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() == m_thread.get_id());

    return m_world;
}

VOLCANO_INLINE void KernelBase::kick(void);

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNELBASE_HPP
