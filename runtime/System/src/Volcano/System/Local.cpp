//
//
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>

VOLCANO_SYSTEM_BEGIN

Local::Local(const std::string& manifest_path)
	: Base(manifest_path)
	, window_("Volcano", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600)
	, acoustics_space_(scene())
	, graphics_renderer_(scene()) {
}

void Local::loadMap(const nlohmann::json& json, entt::handle map) {
	Base::loadMap(json, map);
	runTask([this, &json, map] {

	});
}

void Local::loadEntity(const nlohmann::json& json, entt::handle entity) {
	Base::loadEntity(json, entity);
	runTask([this, &json, entity] {

	});
}

void Local::loadingFrame(Duration elapsed) {
	pollEvents();
	Base::loadingFrame(elapsed);
	// TODO
	render();
}

void Local::readyFrame(Duration elapsed) {
	pollEvents();
	Base::readyFrame(elapsed);
	// TODO
	render();
}

void Local::playingFrame(Duration elapsed) {
	pollEvents();
	Base::playingFrame(elapsed);
	// TODO
	render();
}

void Local::pausedFrame(Duration elapsed) {
	pollEvents();
	Base::pausedFrame(elapsed);
	// TODO
	render();
}

void Local::errorFrame(Duration elapsed) {
	pollEvents();
	Base::loadingFrame(elapsed);
	// TODO
	render();
}

void Local::handleEvent(const SDL_Event& evt) {
	window_.handleEvent(evt);
}

void Local::pollEvents() {
	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		if (evt.type != SDL_QUIT) {
			handleEvent(evt);
		} else {
			stop();
		}
	}
}

void Local::render() {
	if (window_.makeCurrent()) {
		graphics_renderer_.render();
		window_.swapBuffers();
	}

	acoustics_space_.render();
}

VOLCANO_SYSTEM_END
