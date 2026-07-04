//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <async++.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context {
public:
    Context() = default;
    virtual ~Context() = default;

public:
    void schedule(async::task_run_handle rh) {
        scheduler_.schedule(std::move(rh));
    }

    virtual void load(nlohmann::json&& metadata, async::cancellation_token& cancellation);

protected:
    void runAllTasks() {
        scheduler_.run_all_tasks();
    }

private:
    async::fifo_scheduler scheduler_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
