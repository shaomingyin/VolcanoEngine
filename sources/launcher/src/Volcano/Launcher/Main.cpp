//
//
#include <cstdio>
#include <memory>
#include <string_view>
#include <filesystem>
#include <thread>

#include <argh.h>
#include <physfs.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/PhysFileSystem.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Player.hpp>
#include <Volcano/Launcher/SinglePlayer.hpp>
#include <Volcano/Launcher/MultiThreadedSinglePlayer.hpp>
#include <Volcano/Launcher/MultiPlayer.hpp>

VOLCANO_LAUNCHER_BEGIN

static void printVersion(void)
{
	spdlog::info("VolcanoEngine v{}.{}.{}",
		VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);
}

static void printHelp(void)
{
	spdlog::info("Usage:");
	spdlog::info("  volcano-launcher [options] [path]");
	spdlog::info("  [path]:");
	spdlog::info("    Specifies the game root path, default to cwd.");
	spdlog::info("Options:");
	spdlog::info("  -h, --help");
	spdlog::info("    Show this informations.");
	spdlog::info("  -v, --version");
	spdlog::info("    Show version.");
	spdlog::info("  -d, --debug");
	spdlog::info("    Enable debug log level.");
	spdlog::info("  --multiplayer");
	spdlog::info("    Enable multiplayer mode.");
	spdlog::info("  --multithreaded");
	spdlog::info("    Enable multithreaded mode in single player mode.");
}

static int main(int argc, char *argv[])
{
	auto logger = spdlog::stdout_color_mt("Launcher");
	makeLogger(logger);
	spdlog::set_default_logger(logger);

	argh::parser args(argv);

	if (args[{ "-d", "--debug" }])
		spdlog::set_level(spdlog::level::debug);

	printVersion();

	if (args[{ "-v", "--version" }])
		return EXIT_SUCCESS;

	if (args[{ "-h", "--help" }]) {
		printHelp();
		return EXIT_SUCCESS;
	}

	auto rootPath = std::filesystem::current_path().u8string();

	if (args.size() > 1) {
		if (args.size() != 2) {
			spdlog::error("Too many root path.");
			return EXIT_FAILURE;
		}
		rootPath = args[1];
	}

	if (!std::filesystem::is_directory(rootPath)) {
		spdlog::error("Invalid root path '{}', not a directory.", rootPath);
		return EXIT_FAILURE;
	}

	if (!PHYSFS_init(argv[0])) {
		spdlog::error("Failed to init PhysFS.");
		return EXIT_FAILURE;
	}

	ScopeGuard physfsGuard([] { PHYSFS_deinit(); });

	if (!PHYSFS_setWriteDir(rootPath.c_str())) {
		spdlog::error("Failed to set physfs write dir.");
		return EXIT_FAILURE;
	}

	if (!PHYSFS_mount(rootPath.c_str(), "/", 0)) {
		spdlog::error("Failed to mount physfs root dir.");
		return EXIT_FAILURE;
	}

	auto fs = std::make_unique<PhysFileSystem>();
	if (!fs) {
		spdlog::error("Failed to create physfs object.");
		return EXIT_FAILURE;
	}

	spdlog::debug("Initializing SDL...");
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		spdlog::error("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	ScopeGuard sdlGuard([] { SDL_Quit(); });

	std::unique_ptr<Player> player;

	if (args[{ "--multiplayer" }]) {
		spdlog::info("Initializing multiplayer mode...");
		player.reset(new MultiPlayer(fs.get()));
	} else {
		spdlog::info("Initializing singleplayer mode...");
		if (args[{ "--multithreaded" }])
			player.reset(new MultiThreadedSinglePlayer(fs.get()));
		else
			player.reset(new SinglePlayer(fs.get()));
	}

	if (!player || !player->init()) {
		spdlog::error("Failed to init game.");
		return EXIT_FAILURE;
	}

	spdlog::debug("Running main loop...");

	SDL_Event evt;
	TimePoint next = Clock::now();

	while (!player->shouldQuit()) {
		while (SDL_PollEvent(&evt))
			player->handleEvent(evt);
		if (Clock::now() >= next)
			next = player->update();
		else
			std::this_thread::yield();
	}

	spdlog::debug("Exited.");

	return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
	return Volcano::Launcher::main(argc, argv);
}
