//
//
#include <cmath>

#include <Volcano/Physics/SphereCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

SphereCollisionShape::SphereCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float SphereCollisionShape::radius() const noexcept {
    return settings_.mRadius;
}

void SphereCollisionShape::setRadius(float radius) {
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

bool SphereCollisionShape::isValid() const noexcept {
    return std::isfinite(radius()) && radius() > 0.0f;
}

const JPH::SphereShapeSettings& SphereCollisionShape::settings() const noexcept {
    return settings_;
}

VOLCANO_PHYSICS_END
