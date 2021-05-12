//
//
#include <argh.h>
#include <physfs.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

static int main(int argc, char *argv[])
{
	auto logger = spdlog::stdout_color_mt("Server");
	makeLogger(logger);
	spdlog::set_default_logger(logger);

	spdlog::debug("Initializing PhysFS...");
	if (!PHYSFS_init(argv[0])) {
		spdlog::error("Failed to init PhysFS.");
		return EXIT_FAILURE;
	}

	Volcano::ScopeGuard physfsGuard([] () { PHYSFS_deinit(); });

	auto loop = uv_default_loop();
	if (loop == nullptr) {
		spdlog::error("Failed to init libuv.");
		return EXIT_FAILURE;
	}

	spdlog::info("Running...");
	uv_run(loop, UV_RUN_DEFAULT);

	spdlog::info("Exited.");

	return EXIT_SUCCESS;
}

VOLCANO_SERVER_END

int main(int argc, char *argv[])
{
	return Volcano::Server::main(argc, argv);
}
