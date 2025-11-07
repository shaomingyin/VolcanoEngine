//
//
#ifndef VOLCANO_FRAMEWORK_CONTEXT_H
#define VOLCANO_FRAMEWORK_CONTEXT_H

#include <chrono>
#include <string>
#include <system_error>

#include <async++.h>

#include <Volcano/Version.h>
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

    virtual void quit() noexcept = 0;
    virtual unsigned long fps() const noexcept  = 0;
    virtual unsigned long fpsMax() const noexcept = 0;
    virtual void setFpsMax(unsigned long v) noexcept = 0;

protected:
    void runAllTasks() noexcept {
        try {
            scheduler_.run_all_tasks();
        } catch (...) {
        }
    }

private:
    async::fifo_scheduler scheduler_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONTEXT_H
