//
//
#include <Volcano/World/Collision.h>
#include <Volcano/Physics/System.h>

VOLCANO_PHYSICS_BEGIN

System::System(World::Scene& scene)
	: scene_(scene)
	, gravity_(0.0f, -9.81f, 0.0f) {
	scene_.on_construct<World::BoxCollisionShape>().connect<&System::onBoxCollisionShapeConstructed>(this);
	scene_.on_construct<World::SphereCollisionShape>().connect<&System::onSphereCollisionShapeConstructed>(this);
	scene_.on_construct<World::CapsuleCollisionShape>().connect<&System::onCapsuleCollisionShapeConstructed>(this);
	scene_.on_construct<World::PlaneCollisionShape>().connect<&System::onPlaneCollisionShapeConstructed>(this);
	scene_.on_construct<World::MeshCollisionShape>().connect<&System::onMeshCollisionShapeConstructed>(this);
}

System::~System() {
	scene_.on_construct<World::BoxCollisionShape>().disconnect<&System::onBoxCollisionShapeConstructed>(this);
	scene_.on_construct<World::SphereCollisionShape>().disconnect<&System::onSphereCollisionShapeConstructed>(this);
	scene_.on_construct<World::CapsuleCollisionShape>().disconnect<&System::onCapsuleCollisionShapeConstructed>(this);
	scene_.on_construct<World::PlaneCollisionShape>().disconnect<&System::onPlaneCollisionShapeConstructed>(this);
	scene_.on_construct<World::MeshCollisionShape>().disconnect<&System::onMeshCollisionShapeConstructed>(this);
}

void System::enable() noexcept {
	if (!enabled_) {
		enabled_ = true;
		onEnabledChanged(enabled_);
	}
}

void System::disable() noexcept {
	if (enabled_) {
		enabled_ = false;
		onEnabledChanged(enabled_);
	}
}

void System::setGravity(const Eigen::Vector3f& g) noexcept {
	if (!gravity_.isApprox(g, 1e-3f)) {
		gravity_ = g;
		onGravityChanged(gravity_);
	}
}

void System::update(Clock::duration elapsed) noexcept {
	if (enabled_) {
		onUpdate(elapsed);
	}
}

void System::onGravityChanged(const Eigen::Vector3f& v) noexcept {
}

void System::onEnabledChanged(bool enabled) noexcept {
}

void System::onUpdate(Clock::duration elapsed) noexcept {
}

void System::onBoxCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void System::onSphereCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void System::onCapsuleCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void System::onPlaneCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

void System::onMeshCollisionShapeConstructed(entt::registry& reg, entt::entity ent) noexcept {
}

VOLCANO_PHYSICS_END
