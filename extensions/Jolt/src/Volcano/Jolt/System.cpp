//
//
#include <Volcano/Jolt/System.h>

VOLCANO_JOLT_BEGIN

System::System(World::Scene& scene)
	: Physics::System(scene) {
}

System::~System() {
}

void System::onGravityChanged(const Eigen::Vector3f& v) noexcept {
	physics_system_.SetGravity({ v.x(), v.y(), v.z() });
}

void System::onEnabledChanged(bool enabled) noexcept {
	if (enabled) {
		//physics_system_.Activate();
	} else {
		//physics_system_.Deactivate();
	}
}

VOLCANO_JOLT_END
