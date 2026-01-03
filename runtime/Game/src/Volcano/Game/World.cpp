//
//
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World() {
}

World::~World() {
    // TODO loading_task_
}

void World::update(Clock::duration elapsed) {
    scene_.update(elapsed);
}

VOLCANO_GAME_END
