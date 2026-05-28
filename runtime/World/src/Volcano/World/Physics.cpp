//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Volcano/Math.h>
#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

Physics::Physics(entt::registry& registry)
	: registry_(registry)
	, enabled_(false)
	, gravity_({ 0.0f, -9.81f, 0.0f }) {
}

void Physics::enable() noexcept {
	if (enabled_) {
		return;
	}

	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyAdded(ent);
	}

	registry_.on_construct<Transform>().connect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().connect<&Physics::onBodyAdded>(this);

	enabled_ = true;
}

void Physics::disable() noexcept {
	if (enabled_) {
		return;
	}

	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyRemoved(ent);
	}

	registry_.on_construct<Transform>().disconnect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().disconnect<&Physics::onBodyAdded>(this);

	enabled_ = false;
}

void Physics::setGravity(const Eigen::Vector3f& v) noexcept {
	if (!gravity_.isApprox(v)) {
		gravity_ = v;
		physics_system_.SetGravity({ v.x(), v.y(), v.z() });
	}
}

void Physics::update(Clock::duration elapsed) noexcept {
	if (!enabled_) {
		return;
	}

	// physics_system_.Update();

	auto view = registry_.view<Transform, JPH::BodyID>();
	for (auto ent: view) {

	}
}

void Physics::onBodyAdded(entt::entity ent) noexcept {
	auto transform = registry_.try_get<Transform>(ent);
	if (transform == nullptr) {
		return;
	}

	auto settings = registry_.try_get<JPH::BodyCreationSettings>(ent);
	if (settings == nullptr) {
		return;
	}

	auto body_id = body_interface_->CreateAndAddBody(*settings, JPH::EActivation::Activate);
	if (body_id.IsInvalid()) {
		registry_.emplace<JPH::BodyID>(ent, body_id);
	}
}

void Physics::onBodyRemoved(entt::entity ent) noexcept {
	auto body_id = registry_.get<JPH::BodyID>(ent);
	body_interface_->RemoveBody(body_id);
	registry_.remove<JPH::BodyID>(ent);
}

VOLCANO_WORLD_END
