//
//
#include <cmath>

#include <Volcano/World/BoxCollisionShape.h>

VOLCANO_WORLD_BEGIN

BoxCollisionShape::BoxCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

QVector3D BoxCollisionShape::size() const noexcept {
    return size_;
}

void BoxCollisionShape::setSize(const QVector3D& size) {
    if (size_ == size) {
        return;
    }

    const bool wasValid = isValid();
    size_ = size;
    emit sizeChanged(this->size());
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

float BoxCollisionShape::convexRadius() const noexcept {
    return convexRadius_;
}

void BoxCollisionShape::setConvexRadius(float radius) {
    radius = qMax(0.0f, radius);
    if (qFuzzyCompare(convexRadius(), radius)) {
        return;
    }

    const bool wasValid = isValid();
    convexRadius_ = radius;
    emit convexRadiusChanged(radius);
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool BoxCollisionShape::isValid() const noexcept {
    const auto halfExtent = size_ * 0.5f;
    const float radius = convexRadius();
    return std::isfinite(halfExtent.x()) && halfExtent.x() > radius
        && std::isfinite(halfExtent.y()) && halfExtent.y() > radius
        && std::isfinite(halfExtent.z()) && halfExtent.z() > radius;
}

VOLCANO_WORLD_END
