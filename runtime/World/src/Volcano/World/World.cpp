//
//
#include <Volcano/World/World.h>

VOLCANO_WORLD_BEGIN

World::World()
	: scene_(createVolcanoWorldScene()) {
    VOLCANO_ASSERT(scene_);
}

void World::update(Clock::duration elapsed) noexcept {
	scene_->update(elapsed);
}

VOLCANO_WORLD_END
