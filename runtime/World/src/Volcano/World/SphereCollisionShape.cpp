//
//
#include <cmath>

#include <Volcano/World/SphereCollisionShape.h>

VOLCANO_WORLD_BEGIN

SphereCollisionShape::SphereCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

float SphereCollisionShape::radius() const noexcept {
    return radius_;
}

void SphereCollisionShape::setRadius(float radius) {
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

bool SphereCollisionShape::isValid() const noexcept {
    return std::isfinite(radius()) && radius() > 0.0f;
}

VOLCANO_WORLD_END
