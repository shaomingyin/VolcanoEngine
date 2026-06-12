//
//
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/Math.h>
#include <Volcano/World/Physical.h>
#include <Volcano/Physics/System.h>

VOLCANO_PHYSICS_BEGIN

constexpr JPH::uint maxBodies = 1024;
constexpr JPH::uint numBodyMutexes = 0;
constexpr JPH::uint maxBodyPairs = 1024;
constexpr JPH::uint maxContactConstraints = 1024;

System::System(entt::registry& registry)
	: registry_(registry)
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

	registry_.on_construct<Transform>().connect<&System::onBodyAdded>(this);
	registry_.on_construct<World::RigidBody>().connect<&System::onBodyAdded>(this);
}

System::~System() {
	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyRemoved(ent);
	}

	registry_.on_construct<Transform>().disconnect<&System::onBodyAdded>(this);
	registry_.on_construct<World::RigidBody>().disconnect<&System::onBodyAdded>(this);
}

void System::setGravity(const Eigen::Vector3f& v) noexcept {
	if (!gravity_.isApprox(v)) {
		gravity_ = v;
		physics_system_.SetGravity({ v.x(), v.y(), v.z() });
	}
}

void System::update(Clock::duration elapsed) noexcept {
	auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	physics_system_.Update(elapsed_us / 1000000.0f, 1, &temp_allocator_, &job_system_);

	auto view = registry_.view<Transform, JPH::BodyID>();
	for (auto ent: view) {

	}
}

void System::init() {
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();
}

void System::onBodyAdded(entt::entity ent) noexcept {
	auto transform = registry_.try_get<Transform>(ent);
	if (transform == nullptr) {
		return;
	}

	auto rigid_body = registry_.try_get<World::RigidBody>(ent);
	if (rigid_body == nullptr) {
		return;
	}

	JPH::BodyCreationSettings settings;
	// TODO settings from rigid_body...

	auto body_id = body_interface_->CreateAndAddBody(settings, JPH::EActivation::Activate);
	if (body_id.IsInvalid()) {
		registry_.emplace<JPH::BodyID>(ent, body_id);
	}
}

void System::onBodyRemoved(entt::entity ent) noexcept {
	auto body_id = registry_.get<JPH::BodyID>(ent);
	body_interface_->RemoveBody(body_id);
	registry_.remove<JPH::BodyID>(ent);
}

VOLCANO_PHYSICS_END
