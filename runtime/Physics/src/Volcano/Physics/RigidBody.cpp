//
//
#include <utility>

#include <QQmlInfo>
#include <QtQml/qqml.h>

#include <Volcano/Physics/BoxCollisionShape.h>
#include <Volcano/Physics/CapsuleCollisionShape.h>
#include <Volcano/Physics/RigidBody.h>
#include <Volcano/Physics/SphereCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

static_assert(static_cast<JPH::EMotionType>(RigidBody::MotionType::Static) == JPH::EMotionType::Static);
static_assert(static_cast<JPH::EMotionType>(RigidBody::MotionType::Kinematic) == JPH::EMotionType::Kinematic);
static_assert(static_cast<JPH::EMotionType>(RigidBody::MotionType::Dynamic) == JPH::EMotionType::Dynamic);
static_assert(static_cast<JPH::EMotionQuality>(RigidBody::MotionQuality::Discrete) == JPH::EMotionQuality::Discrete);
static_assert(static_cast<JPH::EMotionQuality>(RigidBody::MotionQuality::LinearCast) == JPH::EMotionQuality::LinearCast);

RigidBody::RigidBody(QObject* parent)
    : QObject(parent) {
    settings_.mObjectLayer = 1;
}

void RigidBody::registerQmlTypes() {
    qmlRegisterUncreatableType<CollisionShape>("Volcano.Physics", 1, 0, "CollisionShape",
                                            QStringLiteral("Use a concrete collision shape."));
    qmlRegisterType<RigidBody>("Volcano.Physics", 1, 0, "RigidBody");
    qmlRegisterType<BoxCollisionShape>("Volcano.Physics", 1, 0, "BoxCollisionShape");
    qmlRegisterType<CapsuleCollisionShape>("Volcano.Physics", 1, 0, "CapsuleCollisionShape");
    qmlRegisterType<SphereCollisionShape>("Volcano.Physics", 1, 0, "SphereCollisionShape");
}

CollisionShape* RigidBody::shape() const noexcept {
    return shape_;
}

void RigidBody::setShape(CollisionShape* shape) {
    if (shape_ == shape) {
        return;
    }

    if (shape_) {
        disconnect(shape_, nullptr, this, nullptr);
    }
    shape_ = shape;
    if (shape_) {
        connect(shape_, &CollisionShape::shapeChanged, this, &RigidBody::settingsChanged);
        connect(shape_, &QObject::destroyed, this, [this]() {
            emit shapeChanged(nullptr);
            emit settingsChanged();
        });
    }
    emit shapeChanged(shape_);
    emit settingsChanged();
}

RigidBody::MotionType RigidBody::motionType() const noexcept {
    return static_cast<MotionType>(settings_.mMotionType);
}

void RigidBody::setMotionType(MotionType type) {
    if (motionType() != type) {
        settings_.mMotionType = static_cast<JPH::EMotionType>(type);
        emit motionTypeChanged(type);
        emit settingsChanged();
    }
}

RigidBody::MotionQuality RigidBody::motionQuality() const noexcept {
    return static_cast<MotionQuality>(settings_.mMotionQuality);
}

void RigidBody::setMotionQuality(MotionQuality quality) {
    if (motionQuality() != quality) {
        settings_.mMotionQuality = static_cast<JPH::EMotionQuality>(quality);
        emit motionQualityChanged(quality);
        emit settingsChanged();
    }
}

uint RigidBody::objectLayer() const noexcept {
    return settings_.mObjectLayer;
}

void RigidBody::setObjectLayer(uint layer) {
    if (!std::in_range<JPH::ObjectLayer>(layer)) {
        qmlWarning(this) << "Object layer is outside the Jolt range:" << layer;
        return;
    }

    if (objectLayer() != layer) {
        settings_.mObjectLayer = static_cast<JPH::ObjectLayer>(layer);
        emit objectLayerChanged(layer);
        emit settingsChanged();
    }
}

bool RigidBody::isSensor() const noexcept {
    return settings_.mIsSensor;
}

void RigidBody::setSensor(bool sensor) {
    if (isSensor() != sensor) {
        settings_.mIsSensor = sensor;
        emit sensorChanged(sensor);
        emit settingsChanged();
    }
}

float RigidBody::friction() const noexcept {
    return settings_.mFriction;
}

void RigidBody::setFriction(float friction) {
    if (!qFuzzyCompare(this->friction(), friction)) {
        settings_.mFriction = friction;
        emit frictionChanged(friction);
        emit settingsChanged();
    }
}

float RigidBody::restitution() const noexcept {
    return settings_.mRestitution;
}

void RigidBody::setRestitution(float restitution) {
    if (!qFuzzyCompare(this->restitution(), restitution)) {
        settings_.mRestitution = restitution;
        emit restitutionChanged(restitution);
        emit settingsChanged();
    }
}

float RigidBody::gravityFactor() const noexcept {
    return settings_.mGravityFactor;
}

void RigidBody::setGravityFactor(float factor) {
    if (!qFuzzyCompare(gravityFactor(), factor)) {
        settings_.mGravityFactor = factor;
        emit gravityFactorChanged(factor);
        emit settingsChanged();
    }
}

VOLCANO_PHYSICS_END
