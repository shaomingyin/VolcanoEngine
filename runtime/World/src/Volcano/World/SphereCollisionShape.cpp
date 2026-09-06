//
//
#include <Volcano/World/SphereCollisionShape.h>

#include <Jolt/Physics/Collision/Shape/SphereShape.h>

VOLCANO_WORLD_BEGIN

SphereCollisionShape::SphereCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float SphereCollisionShape::radius() const noexcept {
    return radius_;
}

void SphereCollisionShape::setRadius(float radius) {
    radius = qMax(0.0f, radius);
    if (qFuzzyCompare(radius_, radius)) {
        return;
    }

    const bool wasValid = isValid();
    radius_ = radius;
    emit radiusChanged(radius_);
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool SphereCollisionShape::isValid() const noexcept {
    return radius_ > 0.0f;
}

JPH::ShapeRefC SphereCollisionShape::shape() const {
    if (!isValid()) {
        return {};
    }

    const JPH::SphereShapeSettings settings(radius_);
    const auto result = settings.Create();
    return result.IsValid()? result.Get(): JPH::ShapeRefC();
}

VOLCANO_WORLD_END
