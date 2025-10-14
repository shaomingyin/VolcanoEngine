//
//
#ifndef VOLCANO_FRAMEWORK_CONTEXT_H
#define VOLCANO_FRAMEWORK_CONTEXT_H

#include <string>
#include <chrono>

#include <async++.h>

#include <Volcano/World/Scene.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Context {
public:
    using Clock = std::chrono::steady_clock;
    
public:
    Context() = default;
    virtual ~Context() = default;

public:
    void schedule(async::task_run_handle t) {
        scheduler_.schedule(std::move(t));
    }

    World::Scene& scene() noexcept {
        return scene_;
    }

    virtual void quit() noexcept = 0;
    virtual const std::string& name() const noexcept = 0;
    virtual unsigned long fps() const noexcept  = 0;
    virtual unsigned long fpsMax() const noexcept = 0;
    virtual void setFpsMax(unsigned long v) noexcept = 0;
    virtual bool isPhysicsEnabled() const noexcept = 0;
    virtual void enablePhysics() noexcept = 0;
    virtual void disablePhysics() noexcept = 0;

protected:
    void runAllTasks() noexcept;

private:
    async::fifo_scheduler scheduler_;
    World::Scene scene_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
