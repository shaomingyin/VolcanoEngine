//
//
#include <Volcano/Common.hpp>

void uv_close_sync(uv_handle_t *p)
{
	bool closed = false;
	p->data = &closed;

	uv_close(p, [](uv_handle_t *p) {
		auto closed = reinterpret_cast<bool *>(p->data);
		*closed = true;
	});

	while (!closed)
		uv_run(p->loop, UV_RUN_ONCE);
}

VOLCANO_BEGIN

void makeLogger(std::shared_ptr<spdlog::logger> logger)
{
	VOLCANO_ASSERT(logger);

#ifdef VOLCANO_DEBUG
	logger->set_level(spdlog::level::debug);
#else
	logger->set_level(spdlog::level::info);
#endif

	logger->set_pattern("[Volcano%n] %^%L%$ %T.%e %t %v");
}

VOLCANO_END

#define IPC_IMPLEMENTATION
#include <ipc.h>

#ifdef _WIN32
#	pragma comment(lib, "wsock32.lib")
#endif
