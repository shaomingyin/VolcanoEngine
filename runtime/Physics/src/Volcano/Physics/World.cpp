//
//
#include <Volcano/Physics/World.h>

VOLCANO_PHYSICS_BEGIN

World::World()
    : gravity_(0.0f, -9.8f, 0.0f) {
}

void World::setGravity(const Eigen::Vector3f& v) {
    gravity_ = v;
}

VOLCANO_PHYSICS_END
