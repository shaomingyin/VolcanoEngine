//
//
#include <Volcano/Jolt/World.h>

VOLCANO_JOLT_BEGIN

World::World(Stage::Scene& scene)
	: Physics::World(scene) {
}

World::~World() {
}

void World::onGravityChanged(const Eigen::Vector3f& v) noexcept {
	physics_system_.SetGravity({ v.x(), v.y(), v.z() });
}

void World::onEnabledChanged(bool enabled) noexcept {
	if (enabled) {
		//physics_system_.Activate();
	} else {
		//physics_system_.Deactivate();
	}
}

VOLCANO_JOLT_END
