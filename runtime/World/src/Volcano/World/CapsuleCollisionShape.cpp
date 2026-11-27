//
//
#include <cmath>

#include <Volcano/World/CapsuleCollisionShape.h>

VOLCANO_WORLD_BEGIN

CapsuleCollisionShape::CapsuleCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float CapsuleCollisionShape::radius() const noexcept {
    return radius_;
}

void CapsuleCollisionShape::setRadius(float radius) {
    radius = qMax(0.0f, radius);
    if (qFuzzyCompare(this->radius(), radius)) {
        return;
    }

    const bool wasValid = isValid();
    radius_ = radius;
    emit radiusChanged(radius);
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

    const bool wasValid = isValid();
    cylinderHeight_ = height;
    emit cylinderHeightChanged(cylinderHeight());
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool CapsuleCollisionShape::isValid() const noexcept {
    return std::isfinite(radius_) && radius_ > 0.0f
        && std::isfinite(cylinderHeight_) && cylinderHeight_ >= 0.0f;
}

VOLCANO_WORLD_END
