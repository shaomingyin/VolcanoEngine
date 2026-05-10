//
//
#include <Volcano/Stage/Collision.h>
#include <Volcano/Physics/World.h>

VOLCANO_PHYSICS_BEGIN

World::World(Stage::Scene& scene)
	: scene_(scene)
	, gravity_(0.0f, -9.81f, 0.0f) {
	scene_.on_construct<Stage::BoxCollisionShape>().connect<&World::onBoxCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::SphereCollisionShape>().connect<&World::onSphereCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::CapsuleCollisionShape>().connect<&World::onCapsuleCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::PlaneCollisionShape>().connect<&World::onPlaneCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::MeshCollisionShape>().connect<&World::onMeshCollisionShapeConstructed>(this);
}

World::~World() {
	scene_.on_construct<Stage::BoxCollisionShape>().disconnect<&World::onBoxCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::SphereCollisionShape>().disconnect<&World::onSphereCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::CapsuleCollisionShape>().disconnect<&World::onCapsuleCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::PlaneCollisionShape>().disconnect<&World::onPlaneCollisionShapeConstructed>(this);
	scene_.on_construct<Stage::MeshCollisionShape>().disconnect<&World::onMeshCollisionShapeConstructed>(this);
}

void World::enable() noexcept {
	if (!enabled_) {
		enabled_ = true;
		onEnabledChanged(enabled_);
	}
}

void World::disable() noexcept {
	if (enabled_) {
		enabled_ = false;
		onEnabledChanged(enabled_);
	}
}

void World::setGravity(const Eigen::Vector3f& g) noexcept {
	if (!gravity_.isApprox(g, 1e-3f)) {
		gravity_ = g;
		onGravityChanged(gravity_);
	}
}

void World::update(Clock::duration elapsed) noexcept {
	if (enabled_) {
		onUpdate(elapsed);
	}
}

void World::onGravityChanged(const Eigen::Vector3f& v) noexcept {
}

void World::onEnabledChanged(bool enabled) noexcept {
}

void World::onUpdate(Clock::duration elapsed) noexcept {
}

void World::onBoxCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void World::onSphereCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void World::onCapsuleCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void World::onPlaneCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void World::onMeshCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

VOLCANO_PHYSICS_END
