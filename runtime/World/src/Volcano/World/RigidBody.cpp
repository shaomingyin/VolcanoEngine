//
//
#include <Volcano/World/RigidBody.h>

#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

namespace {

JPH::EMotionType toJoltMotionType(RigidBody::MotionType type) {
    switch (type) {
    case RigidBody::MotionType::Static:
        return JPH::EMotionType::Static;
    case RigidBody::MotionType::Kinematic:
        return JPH::EMotionType::Kinematic;
    case RigidBody::MotionType::Dynamic:
        return JPH::EMotionType::Dynamic;
    }
    Q_UNREACHABLE_RETURN(JPH::EMotionType::Dynamic);
}

JPH::EMotionQuality toJoltMotionQuality(RigidBody::MotionQuality quality) {
    switch (quality) {
    case RigidBody::MotionQuality::Discrete:
        return JPH::EMotionQuality::Discrete;
    case RigidBody::MotionQuality::LinearCast:
        return JPH::EMotionQuality::LinearCast;
    }
    Q_UNREACHABLE_RETURN(JPH::EMotionQuality::Discrete);
}

}

RigidBody::RigidBody(QObject* parent)
    : QObject(parent) {
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
            shape_ = nullptr;
            emit shapeChanged(nullptr);
            emit settingsChanged();
        });
    }
    emit shapeChanged(shape_);
    emit settingsChanged();
}

RigidBody::MotionType RigidBody::motionType() const noexcept {
    return motionType_;
}

void RigidBody::setMotionType(MotionType type) {
    if (motionType_ != type) {
        motionType_ = type;
        emit motionTypeChanged(type);
        emit settingsChanged();
    }
}

RigidBody::MotionQuality RigidBody::motionQuality() const noexcept {
    return motionQuality_;
}

void RigidBody::setMotionQuality(MotionQuality quality) {
    if (motionQuality_ != quality) {
        motionQuality_ = quality;
        emit motionQualityChanged(quality);
        emit settingsChanged();
    }
}

uint RigidBody::objectLayer() const noexcept {
    return objectLayer_;
}

void RigidBody::setObjectLayer(uint layer) {
    if (objectLayer_ != layer) {
        objectLayer_ = layer;
        emit objectLayerChanged(layer);
        emit settingsChanged();
    }
}

bool RigidBody::isSensor() const noexcept {
    return sensor_;
}

void RigidBody::setSensor(bool sensor) {
    if (sensor_ != sensor) {
        sensor_ = sensor;
        emit sensorChanged(sensor);
        emit settingsChanged();
    }
}

float RigidBody::friction() const noexcept {
    return friction_;
}

void RigidBody::setFriction(float friction) {
    if (!qFuzzyCompare(friction_, friction)) {
        friction_ = friction;
        emit frictionChanged(friction);
        emit settingsChanged();
    }
}

float RigidBody::restitution() const noexcept {
    return restitution_;
}

void RigidBody::setRestitution(float restitution) {
    if (!qFuzzyCompare(restitution_, restitution)) {
        restitution_ = restitution;
        emit restitutionChanged(restitution);
        emit settingsChanged();
    }
}

float RigidBody::gravityFactor() const noexcept {
    return gravityFactor_;
}

void RigidBody::setGravityFactor(float factor) {
    if (!qFuzzyCompare(gravityFactor_, factor)) {
        gravityFactor_ = factor;
        emit gravityFactorChanged(factor);
        emit settingsChanged();
    }
}

JPH::BodyCreationSettings RigidBody::bodyCreationSettings() const {
    JPH::BodyCreationSettings settings;
    if (shape_) {
        settings.SetShape(shape_->shape().GetPtr());
    }
    settings.mMotionType = toJoltMotionType(motionType_);
    settings.mMotionQuality = toJoltMotionQuality(motionQuality_);
    settings.mObjectLayer = static_cast<JPH::ObjectLayer>(objectLayer_);
    settings.mIsSensor = sensor_;
    settings.mFriction = friction_;
    settings.mRestitution = restitution_;
    settings.mGravityFactor = gravityFactor_;
    return settings;
}

VOLCANO_WORLD_END
