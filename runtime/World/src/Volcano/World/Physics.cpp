//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/Math.h>
#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

constexpr JPH::uint maxBodies = 1024;
constexpr JPH::uint numBodyMutexes = 0;
constexpr JPH::uint maxBodyPairs = 1024;
constexpr JPH::uint maxContactConstraints = 1024;

Physics::Physics(entt::registry& registry)
	: registry_(registry)
	, enabled_(false)
	, gravity_({ 0.0f, -9.81f, 0.0f })
	, temp_allocator_(10 * 1024 * 1024)
	, job_system_(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1) {
	physics_system_.Init(
		maxBodies,
		numBodyMutexes,
		maxBodyPairs,
		maxContactConstraints,
		broad_phase_layer_interface_,
		object_vs_broad_phase_layer_filter_,
		object_layer_pair_filter_
	);

	body_interface_ = &physics_system_.GetBodyInterface();

	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyAdded(ent);
	}

	registry_.on_construct<Transform>().connect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().connect<&Physics::onBodyAdded>(this);
}

Physics::~Physics() {
	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyRemoved(ent);
	}

	registry_.on_construct<Transform>().disconnect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().disconnect<&Physics::onBodyAdded>(this);
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

	auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	physics_system_.Update(elapsed_us / 1000000.0f, 1, &temp_allocator_, &job_system_);

	auto view = registry_.view<Transform, JPH::BodyID>();
	for (auto ent: view) {

	}
}

void Physics::init() {
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
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
