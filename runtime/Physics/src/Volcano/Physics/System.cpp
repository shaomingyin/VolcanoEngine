//
//
#include <cmath>
#include <stdexcept>
#include <string>

#include <QMutex>
#include <QMutexLocker>
#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyLock.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/EPhysicsUpdateError.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

#include <Volcano/Physics/CollisionShape.h>
#include <Volcano/Physics/System.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

namespace {

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

class BroadPhaseLayerInterface final: public JPH::BroadPhaseLayerInterface {
public:
    JPH::uint GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NumLayers;
    }

    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override {
        Q_ASSERT(layer < ObjectLayers::NumLayers);
        return layer == ObjectLayers::Static? BroadPhaseLayers::Static: BroadPhaseLayers::Moving;
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override {
        return layer == BroadPhaseLayers::Static? "STATIC": "MOVING";
    }
#endif
};

class ObjectLayerPairFilter final: public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer first, JPH::ObjectLayer second) const override {
        return first == ObjectLayers::Moving || second == ObjectLayers::Moving;
    }
};

class ObjectVsBroadPhaseLayerFilter final: public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer objectLayer, JPH::BroadPhaseLayer broadPhaseLayer) const override {
        if (objectLayer == ObjectLayers::Static) {
            return broadPhaseLayer == BroadPhaseLayers::Moving;
        }
        return true;
    }
};

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

class SystemPrivate {
public:
    SystemPrivate()
        : tempAllocator_(10 * 1024 * 1024)
        , jobSystem_(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers) {
        physicsSystem_.Init(10240, 0, 65536, 10240, broadPhaseLayerInterface_, objectVsBroadPhaseLayerFilter_, objectLayerPairFilter_);
    }

    JPH::TempAllocatorImpl tempAllocator_;
    JPH::JobSystemThreadPool jobSystem_;
    BroadPhaseLayerInterface broadPhaseLayerInterface_;
    ObjectLayerPairFilter objectLayerPairFilter_;
    ObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter_;
    JPH::PhysicsSystem physicsSystem_;
};

System::System(QObject* parent)
    : QObject(parent) {
}

System::~System() {
    disable();
}

void System::enable() {
    if (d_) {
        return;
    }

    acquireJolt();
    try {
        d_ = std::make_unique<SystemPrivate>();
        d_->physicsSystem_.SetGravity(toJoltVector(gravity_));
    } catch (...) {
        releaseJolt();
        throw;
    }
    ++generation_;
    emit enabledChanged(true);
}

void System::disable() {
    if (!d_) {
        return;
    }

    d_.reset();
    releaseJolt();
    emit enabledChanged(false);
}

bool System::isEnabled() const noexcept {
    return d_ != nullptr;
}

void System::setEnabled(bool enabled) {
    if (enabled) {
        enable();
    } else {
        disable();
    }
}

const QVector3D& System::gravity() const noexcept {
    return gravity_;
}

void System::setGravity(const QVector3D& gravity) {
    if (gravity_ == gravity) {
        return;
    }

    gravity_ = gravity;
    if (d_) {
        d_->physicsSystem_.SetGravity(toJoltVector(gravity_));
    }
    emit gravityChanged(gravity_);
}

System::BodyId System::createBody(const RigidBody& body, const QVector3D& position, const QQuaternion& rotation) {
    auto& bodies = backend().physicsSystem_.GetBodyInterface();

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

    const auto nativeShape = shape->createShape();
    const auto normalized = rotation.normalized();
    auto settings = body.settings_;
    settings.SetShape(nativeShape.GetPtr());
    settings.mPosition = JPH::RVec3(position.x(), position.y(), position.z());
    settings.mRotation = JPH::Quat(normalized.x(), normalized.y(), normalized.z(), normalized.scalar());
    const auto* nativeBody = bodies.CreateBody(settings);
    if (!nativeBody) {
        throw std::runtime_error("Jolt could not allocate a rigid body");
    }
    return BodyId(this, generation_, nativeBody->GetID().GetIndexAndSequenceNumber());
}

void System::addBody(BodyId body, bool activate) {
    const auto id = checkedBodyId(body);
    auto& bodies = backend().physicsSystem_.GetBodyInterface();
    if (!bodies.IsAdded(id)) {
        bodies.AddBody(id, activate ? JPH::EActivation::Activate : JPH::EActivation::DontActivate);
    }
}

void System::removeBody(BodyId body) {
    const auto id = checkedBodyId(body);
    auto& bodies = backend().physicsSystem_.GetBodyInterface();
    if (bodies.IsAdded(id)) {
        bodies.RemoveBody(id);
    }
}

void System::destroyBody(BodyId body) {
    removeBody(body);
    backend().physicsSystem_.GetBodyInterface().DestroyBody(JPH::BodyID(body.value_));
}

bool System::isBodyAdded(BodyId body) const {
    const auto id = checkedBodyId(body);
    return backend().physicsSystem_.GetBodyInterface().IsAdded(id);
}

System::BodyPose System::bodyPose(BodyId body) const {
    const auto id = checkedBodyId(body);
    JPH::RVec3 position;
    JPH::Quat rotation;
    backend().physicsSystem_.GetBodyInterface().GetPositionAndRotation(id, position, rotation);
    return {
        QVector3D(static_cast<float>(position.GetX()), static_cast<float>(position.GetY()), static_cast<float>(position.GetZ())),
        QQuaternion(rotation.GetW(), rotation.GetX(), rotation.GetY(), rotation.GetZ())
    };
}

void System::update(std::chrono::steady_clock::duration deltaTime, int collisionSteps) {
    if (!d_) {
        return;
    }
    if (deltaTime < std::chrono::steady_clock::duration::zero() || collisionSteps < 1) {
        throw std::invalid_argument("Invalid physics update interval or step count");
    }
    const auto error = d_->physicsSystem_.Update(std::chrono::duration<float>(deltaTime).count(), collisionSteps,
                                                &d_->tempAllocator_, &d_->jobSystem_);
    if (error != JPH::EPhysicsUpdateError::None) {
        throw std::runtime_error("Physics update failed with error flags " + std::to_string(static_cast<unsigned>(error)));
    }
}

SystemPrivate& System::backend() const {
    if (!d_) {
        throw std::runtime_error("Physics must be enabled");
    }
    return *d_;
}

JPH::BodyID System::checkedBodyId(BodyId body) const {
    const auto& system = backend().physicsSystem_;
    if (body.system_ != this || body.generation_ != generation_) {
        throw std::invalid_argument("Rigid body ID does not belong to the current physics system");
    }

    const JPH::BodyID id(body.value_);
    const JPH::BodyLockRead lock(system.GetBodyLockInterface(), id);
    if (!lock.Succeeded()) {
        throw std::invalid_argument("Rigid body ID no longer identifies a live body");
    }
    return id;
}

VOLCANO_PHYSICS_END
