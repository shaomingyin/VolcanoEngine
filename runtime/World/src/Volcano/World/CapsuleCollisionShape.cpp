//
//
#include <Volcano/World/CapsuleCollisionShape.h>

#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

VOLCANO_WORLD_BEGIN

CapsuleCollisionShape::CapsuleCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float CapsuleCollisionShape::radius() const noexcept {
    return radius_;
}

void CapsuleCollisionShape::setRadius(float radius) {
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

float CapsuleCollisionShape::cylinderHeight() const noexcept {
    return cylinderHeight_;
}

void CapsuleCollisionShape::setCylinderHeight(float height) {
    height = qMax(0.0f, height);
    if (qFuzzyCompare(cylinderHeight_, height)) {
        return;
    }

    cylinderHeight_ = height;
    emit cylinderHeightChanged(cylinderHeight_);
    emit shapeChanged();
}

bool CapsuleCollisionShape::isValid() const noexcept {
    return radius_ > 0.0f;
}

JPH::ShapeRefC CapsuleCollisionShape::shape() const {
    if (!isValid()) {
        return {};
    }

    const JPH::CapsuleShapeSettings settings(cylinderHeight_ * 0.5f, radius_);
    const auto result = settings.Create();
    return result.IsValid()? result.Get(): JPH::ShapeRefC();
}

VOLCANO_WORLD_END
