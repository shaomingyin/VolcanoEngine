//
//
#include <cmath>

#include <Volcano/Physics/BoxCollisionShape.h>

VOLCANO_PHYSICS_BEGIN

BoxCollisionShape::BoxCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

QVector3D BoxCollisionShape::size() const noexcept {
    const auto& halfExtent = settings_.mHalfExtent;
    return QVector3D(halfExtent.GetX(), halfExtent.GetY(), halfExtent.GetZ()) * 2.0f;
}

void BoxCollisionShape::setSize(const QVector3D& size) {
    const JPH::Vec3 halfExtent(size.x() * 0.5f, size.y() * 0.5f, size.z() * 0.5f);
    if (settings_.mHalfExtent == halfExtent) {
        return;
    }

    const bool wasValid = isValid();
    settings_.mHalfExtent = halfExtent;
    settings_.ClearCachedResult();
    emit sizeChanged(this->size());
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

float BoxCollisionShape::convexRadius() const noexcept {
    return settings_.mConvexRadius;
}

void BoxCollisionShape::setConvexRadius(float radius) {
    radius = qMax(0.0f, radius);
    if (qFuzzyCompare(convexRadius(), radius)) {
        return;
    }

    const bool wasValid = isValid();
    settings_.mConvexRadius = radius;
    settings_.ClearCachedResult();
    emit convexRadiusChanged(radius);
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool BoxCollisionShape::isValid() const noexcept {
    const auto& halfExtent = settings_.mHalfExtent;
    const float radius = convexRadius();
    return std::isfinite(halfExtent.GetX()) && halfExtent.GetX() > radius
        && std::isfinite(halfExtent.GetY()) && halfExtent.GetY() > radius
        && std::isfinite(halfExtent.GetZ()) && halfExtent.GetZ() > radius;
}

const JPH::BoxShapeSettings& BoxCollisionShape::settings() const noexcept {
    return settings_;
}

VOLCANO_PHYSICS_END
