//
//
#include <cmath>

#include <Volcano/Physics/CapsuleCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

CapsuleCollisionShape::CapsuleCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float CapsuleCollisionShape::radius() const noexcept {
    return settings_.mRadius;
}

void CapsuleCollisionShape::setRadius(float radius) {
    radius = qMax(0.0f, radius);
    if (qFuzzyCompare(this->radius(), radius)) {
        return;
    }

    const bool wasValid = isValid();
    settings_.mRadius = radius;
    settings_.ClearCachedResult();
    emit radiusChanged(radius);
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

float CapsuleCollisionShape::cylinderHeight() const noexcept {
    return settings_.mHalfHeightOfCylinder * 2.0f;
}

void CapsuleCollisionShape::setCylinderHeight(float height) {
    const float halfHeight = qMax(0.0f, height) * 0.5f;
    if (qFuzzyCompare(settings_.mHalfHeightOfCylinder, halfHeight)) {
        return;
    }

    const bool wasValid = isValid();
    settings_.mHalfHeightOfCylinder = halfHeight;
    settings_.ClearCachedResult();
    emit cylinderHeightChanged(cylinderHeight());
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool CapsuleCollisionShape::isValid() const noexcept {
    return settings_.IsValid() && std::isfinite(radius()) && std::isfinite(settings_.mHalfHeightOfCylinder);
}

const JPH::CapsuleShapeSettings& CapsuleCollisionShape::settings() const noexcept {
    return settings_;
}

VOLCANO_PHYSICS_END
