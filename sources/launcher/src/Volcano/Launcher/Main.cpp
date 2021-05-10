//
//
#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/MainWindow.hpp>

VOLCANO_LAUNCHER_BEGIN

static bool running;

static void handleEvent(const SDL_Event &evt, MainWindow *mainWindow)
{
	VOLCANO_ASSERT(mainWindow != nullptr);

	switch (evt.type) {
	case SDL_WINDOWEVENT:
		mainWindow->handleEvent(evt.window);
		break;
	case SDL_QUIT:
		running = false;
		break;
	}
}

static int main(int argc, char *argv[])
{
	auto logger = spdlog::stdout_color_mt("console");
	spdlog::set_default_logger(logger);

	int ret;

	ret = SDL_Init(SDL_INIT_EVERYTHING);
	if (ret < 0) {
		spdlog::error("Failed to init SDL.");
		return EXIT_FAILURE;
	}

	Volcano::ScopeGuard sdlGuard([] () { SDL_Quit(); });

	auto mainWindow = std::make_unique<MainWindow>();
	if (!mainWindow || !mainWindow->init()) {
		spdlog::error("Failed to create main window.");
		return EXIT_FAILURE;
	}

	SDL_Event evt;
	Uint32 curr;
	Uint32 pass;
	Uint32 last = SDL_GetTicks();

	running = true;
	while (running) {
		while (SDL_PollEvent(&evt))
			handleEvent(evt, mainWindow.get());
		curr = SDL_GetTicks();
		pass = curr - last;
		if (pass < 0) {
			last = curr;
			continue;
		}
		if (pass < 16) {
			SDL_Delay(1);
			continue;
		}
		mainWindow->update();
		last = curr;
	}

	return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
	return Volcano::Launcher::main(argc, argv);
}
