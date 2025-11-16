//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <mutex>
#include <deque>
#include <filesystem>

#include <async++.h>

#include <Volcano/List.h>
#include <Volcano/Vm/Common.hpp>
#include <Volcano/Vm/Task.hpp>

VOLCANO_VM_BEGIN

class Kernel {
public:
    Kernel(const std::filesystem::path& initrc);
    virtual ~Kernel();

public:
    uv_loop_t* loop() noexcept {
        return &loop_;
	}

    void schedule(async::task_run_handle t) {
        std::unique_lock<std::mutex> ul(async_task_queue_mutex_);
        async_task_queue_.emplace_back(std::move(t));
        uv_async_send(&async_task_scheduler_);
    }

    void stop() noexcept {
        uv_async_send(&async_stopper_);
    }

    void run();

public:
    static Kernel* fromTask(Task* task) noexcept {
        return reinterpret_cast<Kernel*>(task->data);
    }

    static Kernel* fromState(lua_State* L) noexcept {
        return fromTask(taskFromState(L));
    }

    void addTaskHook(lua_State* L, lua_State* T);
	void removeTaskHook(lua_State* L, lua_State* T);
	void resumeTaskHook(lua_State* T, int n);
	void yieldTaskHook(lua_State* T, int n);

protected:
    void resumeTask(lua_State* T) noexcept;
    virtual void mainLoop(lua_State* L);

private:
    void initSyscalls(lua_State* L);
    void loadInitrc(lua_State* L);
    void schedule(lua_State* L);
    void processAsyncTasks();

private:
    int sysSleep(lua_State* L) noexcept;
    int sysStart(lua_State* L) noexcept;

private:
    std::filesystem::path initrc_;
    uv_loop_t loop_;
    uv_async_t async_stopper_;
    uv_prepare_t scheduler_;
    VolcanoList ready_task_list_;
    uv_async_t async_task_scheduler_;
    std::deque<async::task_run_handle> async_task_queue_;
    std::mutex async_task_queue_mutex_;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
