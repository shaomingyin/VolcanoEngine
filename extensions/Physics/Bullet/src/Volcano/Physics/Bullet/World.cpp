//
//
#include <Volcano/Physics/Bullet/World.h>

VOLCANO_PHYSICS_BULLET_BEGIN

World::World()
    : world_(nullptr) {
}

World::~World() {
}

void World::setGravity(Eigen::Vector3f v) {
    if (world_ != nullptr) {
        world_->setGravity(btVector3(v.x(), v.y(), v.z()));
        Physics::World::setGravity(v);
    }
}

void World::step(std::chrono::steady_clock::duration elapsed) {
    if (VOLCANO_LIKELY(world_ != nullptr)) {
        world_->stepSimulation(std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0);
    }
}

VOLCANO_PHYSICS_BULLET_END
