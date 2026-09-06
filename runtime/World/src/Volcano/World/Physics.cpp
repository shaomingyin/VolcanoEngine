//
//
#include <Volcano/World/Physics.h>

#include <QMutex>
#include <QMutexLocker>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/EPhysicsUpdateError.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

VOLCANO_WORLD_BEGIN

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

class PhysicsPrivate {
public:
    PhysicsPrivate()
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

Physics::Physics(QObject* parent)
    : QObject(parent) {
}

Physics::~Physics() {
    disable();
}

void Physics::enable() {
    if (d_) {
        return;
    }

    acquireJolt();
    try {
        d_ = std::make_unique<PhysicsPrivate>();
        d_->physicsSystem_.SetGravity(toJoltVector(gravity_));
    } catch (...) {
        releaseJolt();
        throw;
    }
    emit enabledChanged(true);
}

void Physics::disable() {
    if (!d_) {
        return;
    }

    d_.reset();
    releaseJolt();
    emit enabledChanged(false);
}

bool Physics::isEnabled() const noexcept {
    return d_ != nullptr;
}

void Physics::setEnabled(bool enabled) {
    if (enabled) {
        enable();
    } else {
        disable();
    }
}

const QVector3D& Physics::gravity() const noexcept {
    return gravity_;
}

void Physics::setGravity(const QVector3D& gravity) {
    if (gravity_ == gravity) {
        return;
    }

    gravity_ = gravity;
    if (d_) {
        d_->physicsSystem_.SetGravity(toJoltVector(gravity_));
    }
    emit gravityChanged(gravity_);
}

JPH::PhysicsSystem* Physics::physicsSystem() noexcept {
    return d_? &d_->physicsSystem_: nullptr;
}

const JPH::PhysicsSystem* Physics::physicsSystem() const noexcept {
    return d_? &d_->physicsSystem_: nullptr;
}

JPH::EPhysicsUpdateError Physics::update(Clock::duration deltaTime, int collisionSteps) {
    if (!d_) {
        return JPH::EPhysicsUpdateError::None;
    }
    Q_ASSERT(deltaTime >= Clock::duration::zero());
    Q_ASSERT(collisionSteps > 0);
    return d_->physicsSystem_.Update(std::chrono::duration<float>(deltaTime).count(), collisionSteps, &d_->tempAllocator_, &d_->jobSystem_);
}

VOLCANO_WORLD_END
