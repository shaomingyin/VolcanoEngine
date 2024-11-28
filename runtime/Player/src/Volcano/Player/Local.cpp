//
//
#include <thread>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Player/Local.h>

VOLCANO_PLAYER_BEGIN

Local::Local(World::Scene& scene, int w, int h)
	: Base(scene)
	, window_(name(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h)
	, acoustics_space_(scene)
	, graphics_renderer_(scene) {
}

void Local::loadScene(const nlohmann::json& json) {
	Base::loadScene(json);
}

void Local::loadEntity(const nlohmann::json& json, entt::handle entity) {
	Base::loadEntity(json, entity);
}

void Local::loadingFrame(Duration elapsed) {
	pollEvents();
	Base::loadingFrame(elapsed);
	update(elapsed);
	render();
}

void Local::readyFrame(Duration elapsed) {
	pollEvents();
	Base::readyFrame(elapsed);
	update(elapsed);
	render();
}

void Local::playingFrame(Duration elapsed) {
	pollEvents();
	Base::playingFrame(elapsed);
	update(elapsed);
	render();
}

void Local::pausedFrame(Duration elapsed) {
	pollEvents();
	Base::pausedFrame(elapsed);
	update(elapsed);
	render();
}

void Local::errorFrame(Duration elapsed) {
	pollEvents();
	Base::errorFrame(elapsed);
	update(elapsed);
	render();
}

void Local::handleEvent(const SDL_Event& evt) {
	window_.handleEvent(evt);
}

void Local::update(Duration elapsed) {
	acoustics_space_.update(elapsed);
	graphics_renderer_.update(elapsed);
}

void Local::render() {
	acoustics_space_.render();
	if (window_.beginFrame()) {
		graphics_renderer_.render();
		window_.endFrame();
	}
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

VOLCANO_PLAYER_END
