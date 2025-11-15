//
//
#ifndef VOLCANO_VM_KERNEL_HPP
#define VOLCANO_VM_KERNEL_HPP

#include <string>
#include <filesystem>

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

    void run();
	void stop();

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
    virtual void mainLoop(lua_State* L);
    virtual void scheduleTrappedTasks(lua_State* L);

private:
    void loadInitrc(lua_State* L);
    static void schedule(lua_State* L, VolcanoList& task_list);

private:
    std::filesystem::path initrc_;
    uv_loop_t loop_;
    uv_async_t async_stop_;
    uv_prepare_t scheduler_;
    VolcanoList ready_task_list_;
	VolcanoList trapped_task_list_;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_KERNEL_HPP
