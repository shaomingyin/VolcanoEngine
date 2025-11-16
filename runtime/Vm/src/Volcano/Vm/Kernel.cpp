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

	ret = uv_async_init(&loop_, &async_stopper_, [](uv_async_t* p) {
		uv_stop(p->loop);
	});
	if (ret != 0) {
		throw std::runtime_error("Failed to initialize uv async stopper");
	}

	ret = uv_async_init(&loop_, &async_task_scheduler_, [](uv_async_t* p) {
		auto k = reinterpret_cast<Kernel*>(p->data);
		k->processAsyncTasks();
	});
	async_task_scheduler_.data = this;
}

Kernel::~Kernel() {
	uv_close(reinterpret_cast<uv_handle_t*>(&async_task_scheduler_), nullptr);
	uv_close(reinterpret_cast<uv_handle_t*>(&async_stopper_), nullptr);
	uv_loop_close(&loop_);
}

void Kernel::run() {
	volcanoListReset(&ready_task_list_);

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

void Kernel::resumeTask(lua_State* T) noexcept {
	auto task = taskFromState(T);
	VOLCANO_ASSERT(task->data == this);
	volcanoListNodeUnlink(&task->node);
	volcanoListAppend(&ready_task_list_, &task->node);
}

void Kernel::mainLoop(lua_State * L) {
	initSyscalls(L);
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
		fromState(L)->schedule(L);
	});
	if (ret != 0) {
		throw std::runtime_error("Failed to start uv prepare for scheduler");
	}

	uv_run(&loop_, UV_RUN_DEFAULT);
}

void Kernel::initSyscalls(lua_State* L) {
	luaL_openlibs(L);

	GlobalTable api(L, "volcano");

	lua_pushcclosure(L, [](lua_State* L) { return fromState(L)->sysSleep(L); }, 0);
	lua_setglobal(L, "sleep");

	lua_pushcclosure(L, [](lua_State* L) { return fromState(L)->sysStart(L); }, 0);
	lua_setglobal(L, "start");
}

void Kernel::loadInitrc(lua_State* L) {
	auto file = PHYSFS_openRead(initrc_.string().c_str());
	if (file == nullptr) {
		luaL_error(L, "Failed to open initrc: %s: %s", initrc_.string().c_str(), PHYSFS_getLastError());
	}
	auto file_guard = scopeGuard([file]() {
		PHYSFS_close(file);
	});

	auto file_size = PHYSFS_fileLength(file);
	if (file_size < 1) {
		luaL_error(L, "Initrc file is empty: %s", initrc_.string().c_str());
	}
	std::string buffer(file_size, '\0');
	auto read_size = PHYSFS_read(file, buffer.data(), 1, file_size);
	if (read_size != file_size) {
		luaL_error(L, "Failed to read initrc file: %s", initrc_.string().c_str());
	}

	lua_State* init = lua_newthread(L);
	if (init == nullptr) {
		luaL_error(L, "Failed to create new lua thread for initrc");
	}
	int ret = luaL_loadbuffer(init, buffer.data(), buffer.size(), "initrc");
	if (ret != LUA_OK) {
		const char* msg = lua_tostring(L, -1);
		luaL_error(L, "Failed to load initrc file: %s: %s", initrc_.string().c_str(), msg ? msg : "(error message not a string)");
	}
}

void Kernel::schedule(lua_State* L) {
	VolcanoListNode* node;
	Task* task;
	int ret;

	while (!volcanoListIsEmpty(&ready_task_list_)) {
		node = volcanoListRemoveFirst(&ready_task_list_);
		task = VOLCANO_MEMBEROF(node, Task, node);
		auto T = taskToState(task);
		ret = lua_resume(T, L, 0, nullptr);
		if (ret != LUA_YIELD && ret != LUA_OK) {
			const char* msg = lua_tostring(T, -1);
			throw std::runtime_error(std::format("Task error: {}", msg ? msg : "(error message not a string)"));
		}
	}
}

void Kernel::processAsyncTasks() {
	std::unique_lock<std::mutex> ul(async_task_queue_mutex_);
	while (!async_task_queue_.empty()) {
		auto task = std::move(async_task_queue_.front());
		async_task_queue_.pop_front();
		task.run();
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
