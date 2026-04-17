//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/World/Transform.h>
#include <Volcano/World/Kinetics.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::update(Clock::duration elapsed) {
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	//physics_system_.Update(float(elapsed_ms) / 1000.0f, 1, );

}

VOLCANO_WORLD_END
