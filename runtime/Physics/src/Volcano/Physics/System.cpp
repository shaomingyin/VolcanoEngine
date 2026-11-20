#include <cmath>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>

#include <Volcano/World/RigidBody.h>
#include <Volcano/Physics/System.h>

VOLCANO_PHYSICS_BEGIN

namespace {
constexpr JPH::uint maxBodies = 1024;
constexpr JPH::uint numBodyMutexes = 0;
constexpr JPH::uint maxBodyPairs = 1024;
constexpr JPH::uint maxContactConstraints = 1024;
std::mutex runtime_mutex;
std::size_t runtime_users = 0;

unsigned int physicsWorkerThreads() noexcept {
    const unsigned int threads = std::thread::hardware_concurrency();
    return threads > 1 ? threads - 1 : 1;
}

JPH::Vec3 toJPH(const Eigen::Vector3f& value) noexcept {
    return JPH::Vec3(value.x(), value.y(), value.z());
}

JPH::Quat toJPH(const Eigen::Quaternionf& value) noexcept {
    return JPH::Quat(value.x(), value.y(), value.z(), value.w());
}

void validateTransform(const Transform& transform) {
    if (!transform.translation().allFinite() || !transform.rotation().coeffs().allFinite() ||
        std::abs(transform.rotation().squaredNorm() - 1.0f) > 1e-4f ||
        transform.scale() != Eigen::Vector3f::Ones()) {
        throw std::invalid_argument("RigidBody requires a finite position, unit rotation and unit scale");
    }
}

JPH::BodyCreationSettings bodySettings(const World::RigidBody& body, const Transform& transform) {
    validateTransform(transform);
    validateTransform(body.offset());
    JPH::EMotionType motion;
    switch (body.motion()) {
    case World::RigidBody::Motion::Static:
        motion = JPH::EMotionType::Static;
        break;
    case World::RigidBody::Motion::Kinematic:
        motion = JPH::EMotionType::Kinematic;
        break;
    case World::RigidBody::Motion::Dynamic:
        motion = JPH::EMotionType::Dynamic;
        break;
    default:
        throw std::invalid_argument("Invalid RigidBody motion type");
    }
    const auto shape = std::visit([](const auto& value) -> JPH::ShapeSettings::ShapeResult {
        using Shape = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Shape, World::RigidBody::Box>) {
            return JPH::BoxShapeSettings(toJPH(value.halfExtents()), 0.0f).Create();
        } else if constexpr (std::is_same_v<Shape, World::RigidBody::Sphere>) {
            return JPH::SphereShapeSettings(value.radius()).Create();
        } else {
            return JPH::CapsuleShapeSettings(value.halfHeight(), value.radius()).Create();
        }
    }, body.shape());
    if (shape.HasError()) {
        throw std::invalid_argument("Invalid RigidBody shape: " + std::string(shape.GetError().c_str()));
    }
    const auto offsetShape = JPH::RotatedTranslatedShapeSettings(toJPH(body.offset().translation()),
        toJPH(body.offset().rotation()), shape.Get()).Create();
    if (offsetShape.HasError()) {
        throw std::invalid_argument("Invalid RigidBody offset shape: " + std::string(offsetShape.GetError().c_str()));
    }
    JPH::BodyCreationSettings settings(offsetShape.Get(), toJPH(transform.translation()),
        toJPH(transform.rotation()), motion,
        motion == JPH::EMotionType::Static ? PhysicsLayers::NON_MOVING : PhysicsLayers::MOVING);
    settings.mFriction = body.friction();
    settings.mRestitution = body.restitution();
    settings.mLinearDamping = body.linearDamping();
    settings.mAngularDamping = body.angularDamping();
    settings.mGravityFactor = body.gravityFactor();
    settings.mOverrideMassProperties = JPH::EOverrideMassProperties::CalculateInertia;
    settings.mMassPropertiesOverride.mMass = body.mass();
    return settings;
}
}

System::System(entt::registry& registry)
    : runtime_((init(), &shutdown))
    , registry_(registry)
    , gravity_({0.0f, -9.81f, 0.0f})
    , temp_allocator_(10 * 1024 * 1024)
    , job_system_(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, physicsWorkerThreads()) {
    physics_system_.Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints,
        broad_phase_layer_interface_, object_vs_broad_phase_layer_filter_, object_layer_pair_filter_);
    body_interface_ = &physics_system_.GetBodyInterface();
    physics_system_.SetGravity(toJPH(gravity_));

    auto rollback = scopeGuard([this] {
        disconnect();
        clearBodies();
    });
    for (const auto entity : registry_.view<Transform, World::RigidBody>()) {
        onBodyAdded(entity);
    }
    synchronize();
    registry_.on_construct<Transform>().connect<&System::onBodyAdded>(this);
    registry_.on_construct<World::RigidBody>().connect<&System::onBodyAdded>(this);
    registry_.on_update<Transform>().connect<&System::onTransformChanged>(this);
    registry_.on_update<World::RigidBody>().connect<&System::onBodyAdded>(this);
    registry_.on_destroy<Transform>().connect<&System::onBodyRemoved>(this);
    registry_.on_destroy<World::RigidBody>().connect<&System::onBodyRemoved>(this);
    rollback.dismiss();
}

System::~System() {
    disconnect();
    clearBodies();
}

void System::setGravity(const Eigen::Vector3f& value) noexcept {
    gravity_ = value;
    physics_system_.SetGravity(toJPH(value));
}

bool System::contains(entt::entity entity) const noexcept {
    return bodies_.contains(entity);
}

void System::synchronize() {
    while (!pending_.empty()) {
        const auto entity = *pending_.begin();
        if (registry_.valid(entity) && registry_.all_of<Transform, World::RigidBody>(entity)) {
            replaceBody(entity);
        }
        pending_.erase(entity);
    }
}

void System::update(Clock::duration elapsed) {
    VOLCANO_ASSERT(elapsed >= Clock::duration::zero());
    synchronize();
    if (elapsed == Clock::duration::zero()) {
        return;
    }
    const float seconds = std::chrono::duration<float>(elapsed).count();
    for (const auto& [entity, id] : bodies_) {
        if (body_interface_->GetMotionType(id) == JPH::EMotionType::Kinematic) {
            const auto& transform = registry_.get<Transform>(entity);
            validateTransform(transform);
            body_interface_->MoveKinematic(id, toJPH(transform.translation()), toJPH(transform.rotation()), seconds);
        }
    }
    physics_system_.Update(seconds, 1, &temp_allocator_, &job_system_);
    for (const auto& [entity, id] : bodies_) {
        auto& transform = registry_.get<Transform>(entity);
        const auto position = body_interface_->GetPosition(id);
        const auto rotation = body_interface_->GetRotation(id);
        transform.setTranslation(Eigen::Vector3f(position.GetX(), position.GetY(), position.GetZ()));
        transform.setRotation(Eigen::Quaternionf(rotation.GetW(), rotation.GetX(), rotation.GetY(), rotation.GetZ()));
    }
}

void System::disconnect() noexcept {
    registry_.on_construct<Transform>().disconnect<&System::onBodyAdded>(this);
    registry_.on_construct<World::RigidBody>().disconnect<&System::onBodyAdded>(this);
    registry_.on_update<Transform>().disconnect<&System::onTransformChanged>(this);
    registry_.on_update<World::RigidBody>().disconnect<&System::onBodyAdded>(this);
    registry_.on_destroy<Transform>().disconnect<&System::onBodyRemoved>(this);
    registry_.on_destroy<World::RigidBody>().disconnect<&System::onBodyRemoved>(this);
}

void System::clearBodies() noexcept {
    for (const auto& [entity, id] : bodies_) {
        body_interface_->RemoveBody(id);
        body_interface_->DestroyBody(id);
    }
    bodies_.clear();
}

void System::init() {
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

void System::shutdown() {
    const std::lock_guard lock(runtime_mutex);
    if (runtime_users != 0 && --runtime_users == 0) {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }
}

void System::replaceBody(entt::entity entity) {
    auto settings = bodySettings(registry_.get<World::RigidBody>(entity), registry_.get<Transform>(entity));
    settings.mUserData = static_cast<JPH::uint64>(entity);
    auto old = bodies_.find(entity);
    if (old != bodies_.end() && settings.mMotionType != JPH::EMotionType::Static &&
        body_interface_->GetMotionType(old->second) != JPH::EMotionType::Static) {
        settings.mLinearVelocity = body_interface_->GetLinearVelocity(old->second);
        settings.mAngularVelocity = body_interface_->GetAngularVelocity(old->second);
    }
    auto* body = body_interface_->CreateBody(settings);
    if (body == nullptr) {
        throw std::runtime_error("Physics body capacity exhausted");
    }
    const auto id = body->GetID();
    auto rollback = scopeGuard([this, id] { body_interface_->DestroyBody(id); });
    if (old != bodies_.end()) {
        body_interface_->RemoveBody(old->second);
        body_interface_->DestroyBody(old->second);
        old->second = id;
    } else {
        bodies_.emplace(entity, id);
    }
    body_interface_->AddBody(id, settings.mMotionType == JPH::EMotionType::Static ?
        JPH::EActivation::DontActivate : JPH::EActivation::Activate);
    rollback.dismiss();
}

void System::onBodyAdded(entt::entity entity) {
    pending_.insert(entity);
}

void System::onTransformChanged(entt::entity entity) {
    const auto found = bodies_.find(entity);
    if (found != bodies_.end() && body_interface_->GetMotionType(found->second) == JPH::EMotionType::Kinematic) {
        return;
    }
    onBodyAdded(entity);
}

void System::onBodyRemoved(entt::entity entity) noexcept {
    pending_.erase(entity);
    const auto found = bodies_.find(entity);
    if (found != bodies_.end()) {
        body_interface_->RemoveBody(found->second);
        body_interface_->DestroyBody(found->second);
        bodies_.erase(found);
    }
}

VOLCANO_PHYSICS_END
