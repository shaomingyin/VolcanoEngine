//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

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
    if (motionType() != type) {
        motionType_ = type;
        emit motionTypeChanged(type);
        emit settingsChanged();
    }
}

RigidBody::MotionQuality RigidBody::motionQuality() const noexcept {
    return motionQuality_;
}

void RigidBody::setMotionQuality(MotionQuality quality) {
    if (motionQuality() != quality) {
        motionQuality_ = quality;
        emit motionQualityChanged(quality);
        emit settingsChanged();
    }
}

uint RigidBody::objectLayer() const noexcept {
    return objectLayer_;
}

void RigidBody::setObjectLayer(uint layer) {
    if (objectLayer() != layer) {
        objectLayer_ = layer;
        emit objectLayerChanged(layer);
        emit settingsChanged();
    }
}

bool RigidBody::isSensor() const noexcept {
    return sensor_;
}

void RigidBody::setSensor(bool sensor) {
    if (isSensor() != sensor) {
        sensor_ = sensor;
        emit sensorChanged(sensor);
        emit settingsChanged();
    }
}

float RigidBody::friction() const noexcept {
    return friction_;
}

void RigidBody::setFriction(float friction) {
    if (!qFuzzyCompare(this->friction(), friction)) {
        friction_ = friction;
        emit frictionChanged(friction);
        emit settingsChanged();
    }
}

float RigidBody::restitution() const noexcept {
    return restitution_;
}

void RigidBody::setRestitution(float restitution) {
    if (!qFuzzyCompare(this->restitution(), restitution)) {
        restitution_ = restitution;
        emit restitutionChanged(restitution);
        emit settingsChanged();
    }
}

float RigidBody::gravityFactor() const noexcept {
    return gravityFactor_;
}

void RigidBody::setGravityFactor(float factor) {
    if (!qFuzzyCompare(gravityFactor(), factor)) {
        gravityFactor_ = factor;
        emit gravityFactorChanged(factor);
        emit settingsChanged();
    }
}

VOLCANO_WORLD_END
