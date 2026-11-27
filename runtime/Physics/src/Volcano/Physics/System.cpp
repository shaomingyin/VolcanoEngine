//
//
#include <chrono>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QPointer>
#include <QScopeGuard>
#include <QThread>
#include <QVariant>
#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/EPhysicsUpdateError.h>
#include <Jolt/RegisterTypes.h>

#include <Volcano/Physics/System.h>
#include <Volcano/World/BoxCollisionShape.h>
#include <Volcano/World/CapsuleCollisionShape.h>
#include <Volcano/World/RigidBody.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SphereCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

namespace {

constexpr char bodyBindingKey[] = "volcano.physics.bodyBinding";

struct BodyBinding {
    QPointer<System> system;
    std::uint64_t generation = 0;
    JPH::BodyID id;
    QMetaObject::Connection onDestroyed;
};

static_assert(static_cast<JPH::EMotionType>(World::RigidBody::MotionType::Static) == JPH::EMotionType::Static);
static_assert(static_cast<JPH::EMotionType>(World::RigidBody::MotionType::Kinematic) == JPH::EMotionType::Kinematic);
static_assert(static_cast<JPH::EMotionType>(World::RigidBody::MotionType::Dynamic) == JPH::EMotionType::Dynamic);
static_assert(static_cast<JPH::EMotionQuality>(World::RigidBody::MotionQuality::Discrete) == JPH::EMotionQuality::Discrete);
static_assert(static_cast<JPH::EMotionQuality>(World::RigidBody::MotionQuality::LinearCast) == JPH::EMotionQuality::LinearCast);

namespace ObjectLayers {
constexpr JPH::ObjectLayer Static = 0;
constexpr JPH::ObjectLayer Moving = 1;
constexpr JPH::uint NumLayers = 2;
}

namespace BroadPhaseLayers {
constexpr JPH::BroadPhaseLayer Static(0);
constexpr JPH::BroadPhaseLayer Moving(1);
constexpr JPH::uint NumLayers = 2;
}

JPH::Vec3 toJoltVector(const QVector3D& vector) {
    return JPH::Vec3(vector.x(), vector.y(), vector.z());
}

JPH::ShapeRefC createShape(const World::CollisionShape& shape) {
    if (!shape.isValid()) {
        throw std::invalid_argument("Invalid collision shape configuration");
    }

    JPH::ShapeSettings::ShapeResult result;
    if (const auto* box = qobject_cast<const World::BoxCollisionShape*>(&shape)) {
        result = JPH::BoxShapeSettings(toJoltVector(box->size() * 0.5f), box->convexRadius()).Create();
    } else if (const auto* sphere = qobject_cast<const World::SphereCollisionShape*>(&shape)) {
        result = JPH::SphereShapeSettings(sphere->radius()).Create();
    } else if (const auto* capsule = qobject_cast<const World::CapsuleCollisionShape*>(&shape)) {
        result = JPH::CapsuleShapeSettings(capsule->cylinderHeight() * 0.5f, capsule->radius()).Create();
    } else {
        throw std::invalid_argument("Collision shape is not supported by Physics");
    }

    if (!result.IsValid()) {
        throw std::runtime_error(result.HasError()? result.GetError().c_str(): "Jolt did not create a collision shape");
    }
    return result.Get();
}

QMutex joltMutex;
qsizetype joltInstanceCount = 0;
bool ownsJoltFactory = false;

void acquireJolt() {
    QMutexLocker locker(&joltMutex);
    if (joltInstanceCount++ != 0) {
        return;
    }

    JPH::RegisterDefaultAllocator();
    if (JPH::Factory::sInstance == nullptr) {
        JPH::Factory::sInstance = new JPH::Factory();
        ownsJoltFactory = true;
    }
    JPH::RegisterTypes();
}

void releaseJolt() {
    QMutexLocker locker(&joltMutex);
    Q_ASSERT(joltInstanceCount > 0);
    if (--joltInstanceCount != 0) {
        return;
    }

    JPH::UnregisterTypes();
    if (ownsJoltFactory) {
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
        ownsJoltFactory = false;
    }
}

}

JPH::uint System::BroadPhaseLayerInterface::GetNumBroadPhaseLayers() const {
    return BroadPhaseLayers::NumLayers;
}

JPH::BroadPhaseLayer System::BroadPhaseLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer layer) const {
    Q_ASSERT(layer < ObjectLayers::NumLayers);
    return layer == ObjectLayers::Static? BroadPhaseLayers::Static: BroadPhaseLayers::Moving;
}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
const char* System::BroadPhaseLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const {
    return layer == BroadPhaseLayers::Static? "STATIC": "MOVING";
}
#endif

bool System::ObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer first, JPH::ObjectLayer second) const {
    return first == ObjectLayers::Moving || second == ObjectLayers::Moving;
}

bool System::ObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer objectLayer,
                                                         JPH::BroadPhaseLayer broadPhaseLayer) const {
    if (objectLayer == ObjectLayers::Static) {
        return broadPhaseLayer == BroadPhaseLayers::Moving;
    }
    return true;
}

System::Backend::Backend()
    : tempAllocator(10 * 1024 * 1024)
    , jobSystem(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers) {
    physicsSystem.Init(10240, 0, 65536, 10240, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
}

System::System(World::Scene& scene, QObject* parent)
    : QObject(parent)
    , scene_(scene) {
    Q_ASSERT(scene_.thread() == thread());
    connect(&scene_, &World::Scene::componentAdded, this, &System::attachSceneBody);
    connect(&scene_, &World::Scene::componentRemoved, this, &System::detachSceneBody);
    connect(&scene_, &World::Scene::gravityChanged, this, [this](const QVector3D& gravity) {
        if (backend_) {
            backend_->physicsSystem.SetGravity(toJoltVector(gravity));
        }
    });
}

System::~System() {
    disconnect(&scene_, nullptr, this, nullptr);
    disable();
}

void System::enable() {
    if (backend_) {
        return;
    }

    acquireJolt();
    try {
        backend_.emplace();
        backend_->physicsSystem.SetGravity(toJoltVector(scene_.gravity()));
    } catch (...) {
        backend_.reset();
        releaseJolt();
        throw;
    }
    ++generation_;
    visitSceneComponents(&System::attachSceneBody);
    emit enabledChanged(true);
}

void System::disable() {
    if (!backend_) {
        return;
    }

    visitSceneComponents(&System::detachSceneBody);
    backend_.reset();
    releaseJolt();
    emit enabledChanged(false);
}

bool System::isEnabled() const noexcept {
    return backend_.has_value();
}

void System::setEnabled(bool enabled) {
    if (enabled) {
        enable();
    } else {
        disable();
    }
}

void System::visitSceneComponents(void (System::*handler)(World::Entity*, QObject*)) {
    if (!backend_) {
        return;
    }
    for (auto* entity: scene_.entities()) {
        for (auto* component: entity->components()) {
            (this->*handler)(entity, component);
        }
    }
}

void System::attachSceneBody(World::Entity* entity, QObject* component) {
    auto* body = qobject_cast<World::RigidBody*>(component);
    if (!backend_ || !body) {
        return;
    }

    try {
        const auto binding = body->property(bodyBindingKey).value<BodyBinding>();
        if (binding.system != this || binding.generation != generation_) {
            const auto* transform = entity->transform();
            createSceneBody(*body, transform->translation(), transform->rotation());
        }
    } catch (const std::exception& error) {
        qWarning() << "Could not attach a scene rigid body:" << error.what();
    }
}

void System::detachSceneBody(World::Entity*, QObject* component) {
    auto* body = qobject_cast<World::RigidBody*>(component);
    if (!backend_ || !body) {
        return;
    }
    const auto binding = body->property(bodyBindingKey).value<BodyBinding>();
    if (binding.system == this && binding.generation == generation_) {
        disconnect(binding.onDestroyed);
        destroyNativeBody(binding.id);
        body->setProperty(bodyBindingKey, QVariant{});
    }
}

void System::createSceneBody(World::RigidBody& body, const QVector3D& position, const QQuaternion& rotation) {
    auto& bodies = backend_->physicsSystem.GetBodyInterface();

    const auto previous = body.property(bodyBindingKey).value<BodyBinding>();
    if (previous.system && previous.system->isEnabled() && previous.generation == previous.system->generation_) {
        throw std::invalid_argument("RigidBody already belongs to a physics system");
    }

    const auto* shape = body.shape();
    if (!shape) {
        throw std::invalid_argument("RigidBody requires a collision shape");
    }
    if (body.objectLayer() >= ObjectLayers::NumLayers) {
        throw std::invalid_argument("RigidBody object layer is not supported by Physics");
    }
    const float rotationLength = rotation.lengthSquared();
    if (!std::isfinite(position.x()) || !std::isfinite(position.y()) || !std::isfinite(position.z())
        || !std::isfinite(rotationLength) || qFuzzyIsNull(rotationLength)) {
        throw std::invalid_argument("Invalid rigid body pose");
    }

    const auto nativeShape = createShape(*shape);
    const auto normalized = rotation.normalized();
    JPH::BodyCreationSettings settings(
        nativeShape.GetPtr(),
        JPH::RVec3(position.x(), position.y(), position.z()),
        JPH::Quat(normalized.x(), normalized.y(), normalized.z(), normalized.scalar()),
        static_cast<JPH::EMotionType>(body.motionType()),
        static_cast<JPH::ObjectLayer>(body.objectLayer()));
    settings.mMotionQuality = static_cast<JPH::EMotionQuality>(body.motionQuality());
    settings.mIsSensor = body.isSensor();
    settings.mFriction = body.friction();
    settings.mRestitution = body.restitution();
    settings.mGravityFactor = body.gravityFactor();
    const auto id = bodies.CreateAndAddBody(settings, JPH::EActivation::Activate);
    if (id.IsInvalid()) {
        throw std::runtime_error("Jolt could not allocate a rigid body");
    }
    disconnect(previous.onDestroyed);
    QMetaObject::Connection onDestroyed;
    auto rollback = qScopeGuard([this, id, &onDestroyed] {
        disconnect(onDestroyed);
        destroyNativeBody(id);
    });
    onDestroyed = connect(&body, &QObject::destroyed, this, [this, id, generation = generation_] {
        if (backend_ && generation == generation_) {
            destroyNativeBody(id);
        }
    });
    body.setProperty(bodyBindingKey, QVariant::fromValue(BodyBinding{this, generation_, id, onDestroyed}));
    rollback.dismiss();
}

void System::destroyNativeBody(JPH::BodyID id) {
    auto& bodies = backend_->physicsSystem.GetBodyInterface();
    if (bodies.IsAdded(id)) {
        bodies.RemoveBody(id);
    }
    bodies.DestroyBody(id);
}

void System::update(World::Clock::duration deltaTime, int collisionSteps) {
    Q_ASSERT(scene_.thread() == QThread::currentThread());
    if (!backend_) {
        return;
    }
    if (deltaTime < World::Clock::duration::zero() || collisionSteps < 1) {
        throw std::invalid_argument("Invalid physics update interval or step count");
    }
    const auto error = backend_->physicsSystem.Update(std::chrono::duration<float>(deltaTime).count(), collisionSteps,
                                                      &backend_->tempAllocator, &backend_->jobSystem);
    if (error != JPH::EPhysicsUpdateError::None) {
        throw std::runtime_error("Physics update failed with error flags " + std::to_string(static_cast<unsigned>(error)));
    }
    syncSceneTransforms();
}

void System::syncSceneTransforms() {
    struct TransformUpdate {
        QPointer<World::Entity> entity;
        QPointer<World::RigidBody> body;
        JPH::BodyID id;
        QVector3D translation;
        QQuaternion rotation;
    };

    // Transform notifications can change scene membership or disable Physics.
    std::vector<TransformUpdate> updates;
    const auto& bodies = backend_->physicsSystem.GetBodyInterface();
    for (auto* entity: scene_.entities()) {
        const auto* transform = entity->transform();
        for (auto* component: entity->components()) {
            auto* body = qobject_cast<World::RigidBody*>(component);
            if (!body) {
                continue;
            }
            const auto binding = body->property(bodyBindingKey).value<BodyBinding>();
            if (binding.system != this || binding.generation != generation_
                || bodies.GetMotionType(binding.id) != JPH::EMotionType::Dynamic) {
                continue;
            }
            JPH::RVec3 position;
            JPH::Quat orientation;
            bodies.GetPositionAndRotation(binding.id, position, orientation);
            const QVector3D translation(static_cast<float>(position.GetX()), static_cast<float>(position.GetY()),
                                        static_cast<float>(position.GetZ()));
            const QQuaternion rotation(orientation.GetW(), orientation.GetX(), orientation.GetY(), orientation.GetZ());
            if (transform->translation() != translation || transform->rotation() != rotation) {
                updates.push_back({entity, body, binding.id, translation, rotation});
            }
            break;
        }
    }

    const auto isStillBound = [this, generation = generation_](const TransformUpdate& update) {
        if (!backend_ || generation_ != generation || !update.entity || !update.body) {
            return false;
        }
        const auto binding = update.body->property(bodyBindingKey).value<BodyBinding>();
        return binding.system == this && binding.generation == generation && binding.id == update.id;
    };
    for (const auto& update: updates) {
        if (!isStillBound(update)) {
            continue;
        }
        auto* transform = update.entity->transform();
        if (transform->translation() != update.translation) {
            transform->setTranslation(update.translation);
            if (!isStillBound(update)) {
                continue;
            }
        }
        transform->setRotation(update.rotation);
    }
}

VOLCANO_PHYSICS_END
