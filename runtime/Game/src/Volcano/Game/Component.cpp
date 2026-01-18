//
//
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

Component::Component() {
}

Component::~Component() {
}

void Component::update(Clock::duration elapsed, Scheduler& scheduler) {
}

Graphics::VisibleSet Component::buildVisibleSet(Scheduler& scheduler) const {
	return Graphics::VisibleSet();
}

VOLCANO_GAME_END
