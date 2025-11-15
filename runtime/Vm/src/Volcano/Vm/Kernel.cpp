//
//
#include <stdexcept>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Vm/Kernel.hpp>

VOLCANO_VM_BEGIN

Kernel::Kernel(const std::filesystem::path& initrc)
	: initrc_(initrc) {
	int ret = uv_loop_init(&loop_);
	if (ret != 0) {
		throw std::runtime_error("Failed to initialize uv loop");
	}

	ret = uv_async_init(&loop_, &async_stop_, [](uv_async_t* p) {
		uv_stop(p->loop);
	});
	if (ret != 0) {
		throw std::runtime_error("Failed to initialize uv async stopper");
	}
}

Kernel::~Kernel() {
	uv_loop_close(&loop_);
}

void Kernel::run() {
	volcanoListReset(&ready_task_list_);
	volcanoListReset(&trapped_task_list_);

	lua_State* L = luaL_newstate();
	if (L == nullptr) {
		throw std::runtime_error("cannot create state: not enough memory");
	}
	auto lua_guard = scopeGuard([L]() {
		lua_close(L);
	});
	taskFromState(L)->data = this;

	lua_gc(L, LUA_GCSTOP);
	lua_pushcfunction(L, [](lua_State* L) {
		Kernel* k = reinterpret_cast<Kernel*>(lua_touserdata(L, 1));
		try {
			k->mainLoop(L);
			return 0;
		} catch (const std::exception& e) {
			return luaL_error(L, "exception occurred in vm kernel main loop: %s", e.what());
		} catch (...) {
			return luaL_error(L, "unknown exception occurred in vm kernel main loop.");
		}
	});

	lua_pushlightuserdata(L, this);

	int ret = lua_pcall(L, 1, 0, 0);
	if (ret != LUA_OK) {
		throw std::runtime_error("failed to run vm kernel in protected env.");
	}
}

void Kernel::stop() {
	uv_async_send(&async_stop_);
}

void Kernel::addTaskHook(lua_State* L, lua_State* T) {
	auto task = taskFromState(T);
	task->data = this;
	uv_timer_init(&loop_, &task->sleep_timer);
	volcanoListReset(&task->wait_list);
	volcanoListNodeReset(&task->node);
	volcanoListAppend(&ready_task_list_, &task->node);
}

void Kernel::removeTaskHook(lua_State* L, lua_State* T) {
	auto task = taskFromState(T);
	VOLCANO_ASSERT(task->data == this);
	volcanoListNodeUnlink(&task->node);
}

void Kernel::resumeTaskHook(lua_State* T, int n) {
	auto task = taskFromState(T);
	VOLCANO_ASSERT(task->data == this);
}

void Kernel::yieldTaskHook(lua_State* T, int n) {
	auto task = taskFromState(T);
	VOLCANO_ASSERT(task->data == this);
}

void Kernel::mainLoop(lua_State * L) {
	luaL_openlibs(L);
	loadInitrc(L);

	uv_prepare_t scheduler;
	int ret = uv_prepare_init(&loop_, &scheduler);
	if (ret != 0) {
		throw std::runtime_error("Failed to initialize uv prepare for scheduler");
	}
	auto scheduler_guard = scopeGuard([&scheduler]() {
		uv_close(reinterpret_cast<uv_handle_t*>(&scheduler), nullptr);
	});
	scheduler.data = L;
	ret = uv_prepare_start(&scheduler, [](uv_prepare_t* handle) {
		lua_State* L = reinterpret_cast<lua_State*>(handle->data);
		auto k = fromState(L);
		k->schedule(L, k->ready_task_list_);
		k->schedule(L, k->trapped_task_list_);
	});
	if (ret != 0) {
		throw std::runtime_error("Failed to start uv prepare for scheduler");
	}

	uv_run(&loop_, UV_RUN_DEFAULT);
}

void Kernel::scheduleTrappedTasks(lua_State* L) {
	VolcanoListNode* node;
	Task* task;
	int ret;
	while (!volcanoListIsEmpty(&trapped_task_list_)) {
		node = volcanoListRemoveFirst(&trapped_task_list_);
		task = VOLCANO_MEMBEROF(node, Task, node);
		// TODO
		volcanoListAppend(&ready_task_list_, &task->node);
	}
}

void Kernel::loadInitrc(lua_State* L) {
	auto file = PHYSFS_openRead(initrc_.string().c_str());
	if (file == nullptr) {
		throw std::runtime_error(std::format("Failed to open initrc: {}: {}", initrc_.string().c_str(), PHYSFS_getLastError()));
	}
	auto file_guard = scopeGuard([file]() {
		PHYSFS_close(file);
	});

	auto file_size = PHYSFS_fileLength(file);
	if (file_size < 1) {
		throw std::runtime_error(std::format("Initrc file is empty: {}", initrc_.string().c_str()));
	}
	std::string buffer(file_size, '\0');
	auto read_size = PHYSFS_read(file, buffer.data(), 1, file_size);
	if (read_size != file_size) {
		throw std::runtime_error(std::format("Failed to read initrc file: {}", initrc_.string().c_str()));
	}

	lua_State* init = lua_newthread(L);
	if (init == nullptr) {
		throw std::runtime_error("Failed to create new lua thread for initrc");
	}
	int ret = luaL_loadbuffer(init, buffer.data(), buffer.size(), "initrc");
	if (ret != LUA_OK) {
		const char* msg = lua_tostring(L, -1);
		throw std::runtime_error(std::format("Failed to load initrc file: {}: {}", initrc_.string().c_str(), msg ? msg : "(error message not a string)"));
	}
}

void Kernel::schedule(lua_State* L, VolcanoList& task_list) {
	VolcanoListNode* node;
	Task* task;
	int ret;

	while (!volcanoListIsEmpty(&task_list)) {
		node = volcanoListRemoveFirst(&task_list);
		task = VOLCANO_MEMBEROF(node, Task, node);
		ret = lua_resume(taskToState(task), L, 0, nullptr);
		if (ret != LUA_YIELD && ret != LUA_OK) {
			const char* msg = lua_tostring(taskToState(task), -1);
			throw std::runtime_error(std::format("Task error: {}", msg ? msg : "(error message not a string)"));
		}
	}
}

VOLCANO_VM_END

extern "C" void volcanoVmInitHook(lua_State* L) {
}

extern "C" void volcanoVmShutdownHook(lua_State* L) {
}

extern "C" void volcanoVmAddTaskHook(lua_State* L, lua_State* T) {
	Volcano::Vm::Kernel::fromState(L)->addTaskHook(L, T);
}

extern "C" void volcanoVmRemoveTaskHook(lua_State* L, lua_State* T) {
	Volcano::Vm::Kernel::fromState(L)->removeTaskHook(L, T);
}

extern "C" void volcanoVmResumeTaskHook(lua_State* T, int n) {
	Volcano::Vm::Kernel::fromState(T)->resumeTaskHook(T, n);
}

extern "C" void volcanoVmYieldTaskHook(lua_State* T, int n) {
	Volcano::Vm::Kernel::fromState(T)->yieldTaskHook(T, n);
}
