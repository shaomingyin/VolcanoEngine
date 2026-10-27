//
//
#include <memory>
#include <mutex>
#include <thread>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>

#include <Volcano/World/Physics.h>

VOLCANO_WORLD_BEGIN

constexpr JPH::uint maxBodies = 1024;
constexpr JPH::uint numBodyMutexes = 0;
constexpr JPH::uint maxBodyPairs = 1024;
constexpr JPH::uint maxContactConstraints = 1024;

unsigned int physicsWorkerThreads() noexcept {
    const unsigned int threads = std::thread::hardware_concurrency();
    return threads > 1 ? threads - 1 : 1;
}

Physics::Physics(entt::registry& registry)
	: registry_(registry)
	, gravity_({ 0.0f, -9.81f, 0.0f })
	, temp_allocator_(10 * 1024 * 1024)
	, job_system_(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, physicsWorkerThreads()) {
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
	physics_system_.SetGravity(toJPH(gravity_));

	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyAdded(ent);
	}

	registry_.on_construct<Transform>().connect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().connect<&Physics::onBodyAdded>(this);

	registry_.on_destroy<Transform>().connect<&Physics::onBodyRemoved>(this);
	registry_.on_destroy<JPH::BodyCreationSettings>().connect<&Physics::onBodyRemoved>(this);

	registry_.on_update<JPH::BodyCreationSettings>().connect<&Physics::onBodyChanged>(this);
}

Physics::~Physics() {
	auto view = registry_.view<Transform, JPH::BodyCreationSettings>();
	for (auto ent: view) {
		onBodyRemoved(ent);
	}

	registry_.on_construct<Transform>().disconnect<&Physics::onBodyAdded>(this);
	registry_.on_construct<JPH::BodyCreationSettings>().disconnect<&Physics::onBodyAdded>(this);

	registry_.on_destroy<Transform>().disconnect<&Physics::onBodyRemoved>(this);
	registry_.on_destroy<JPH::BodyCreationSettings>().disconnect<&Physics::onBodyRemoved>(this);

	registry_.on_update<JPH::BodyCreationSettings>().disconnect<&Physics::onBodyChanged>(this);
}

namespace {
std::mutex runtime_mutex;
std::size_t runtime_users = 0;
}

void Physics::init() {
    const std::lock_guard lock(runtime_mutex);
    if (runtime_users == 0) {
        JPH::RegisterDefaultAllocator();
        auto factory = std::make_unique<JPH::Factory>();
        JPH::Factory::sInstance = factory.get();
        JPH::RegisterTypes();
        factory.release();
    }
    ++runtime_users;
}

void Physics::shutdown() {
    const std::lock_guard lock(runtime_mutex);
    if (runtime_users != 0 && --runtime_users == 0) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
}

void Physics::setGravity(const Eigen::Vector3f& v) noexcept {
	if (gravity_ != v) {
		gravity_ = v;
		physics_system_.SetGravity({ v.x(), v.y(), v.z() });
	}
}

void Physics::update(Clock::duration elapsed) noexcept {
	auto elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	physics_system_.Update(elapsed_us / 1000000.0f, 1, &temp_allocator_, &job_system_);

	auto view = registry_.view<Transform, JPH::BodyID>();
	for (auto ent: view) {
		auto& transform = view.get<Transform>(ent);
		const auto body_id = view.get<JPH::BodyID>(ent);
		if (body_id.IsInvalid()) {
			continue;
		}

		const auto position = body_interface_->GetPosition(body_id);
		transform.translation() = Eigen::Vector3f(position.GetX(), position.GetY(), position.GetZ());
		transform.rotation() = fromJPH(body_interface_->GetRotation(body_id));
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

	if (registry_.all_of<JPH::BodyID>(ent)) {
		return;
	}

	JPH::BodyCreationSettings body_settings = *settings;
	body_settings.mPosition = toJPH(transform->translation());
	body_settings.mRotation = toJPH(transform->rotation());
	body_settings.mUserData = static_cast<JPH::uint64>(ent);

	if (const auto* shape_settings = body_settings.GetShapeSettings(); shape_settings != nullptr) {
		const_cast<JPH::ShapeSettings*>(shape_settings)->ClearCachedResult();
	}

	if (body_settings.GetShape() == nullptr) {
		spdlog::warn("Cannot create physics body for entity {} without a shape.", static_cast<JPH::uint64>(ent));
		return;
	}

	const auto body_id = body_interface_->CreateAndAddBody(body_settings, JPH::EActivation::Activate);
	if (!body_id.IsInvalid()) {
		registry_.emplace<JPH::BodyID>(ent, body_id);
	}
}

void Physics::onBodyRemoved(entt::entity ent) noexcept {
	auto body_id = registry_.try_get<JPH::BodyID>(ent);
	if (body_id == nullptr || body_id->IsInvalid()) {
		return;
	}

	body_interface_->RemoveBody(*body_id);
	body_interface_->DestroyBody(*body_id);
	registry_.remove<JPH::BodyID>(ent);
}

void Physics::onBodyChanged(entt::entity ent) noexcept {
	onBodyRemoved(ent);
	onBodyAdded(ent);
}

VOLCANO_WORLD_END
