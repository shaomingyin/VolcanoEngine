//
//
#include <Volcano/System/LocalPlayer.h>

VOLCANO_SYSTEM_BEGIN

LocalPlayer::LocalPlayer()
	: frontend_(resourceManager()) {
}

void LocalPlayer::mainLoop() {
	auto frontend_title = readConfig<std::string>("/frontend/title", "Untitled");
	auto frontend_width = readConfig<int>("/frontend/width", 1024);
	auto frontend_height = readConfig<int>("/frontend/height", 768);
	auto frontend_color_depth = readConfig<int>("/frontend/color_depth", 32);

	if (!frontend_.init(frontend_title, frontend_width, frontend_height)) {
		return;
	}

	frontend_.bind(gameWorld());

	Base::mainLoop();
}

void LocalPlayer::handleEvent(const SDL_Event& evt) {
	frontend_.handleEvent(evt);
}

void LocalPlayer::frame(Duration elapsed) {
	gameWorld()->frame(elapsed);
	frontend_.frame(elapsed);
}

VOLCANO_SYSTEM_END
