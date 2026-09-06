//
//
#include <Volcano/World/BoxCollisionShape.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>

VOLCANO_WORLD_BEGIN

BoxCollisionShape::BoxCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

const QVector3D& BoxCollisionShape::size() const noexcept {
    return size_;
}

void BoxCollisionShape::setSize(const QVector3D& size) {
    if (size_ == size) {
        return;
    }

    const bool wasValid = isValid();
    size_ = size;
    emit sizeChanged(size_);
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
    if (qFuzzyCompare(convexRadius_, radius)) {
        return;
    }

    const bool wasValid = isValid();
    convexRadius_ = radius;
    emit convexRadiusChanged(convexRadius_);
    emit shapeChanged();
    if (wasValid != isValid()) {
        emit validChanged(isValid());
    }
}

bool BoxCollisionShape::isValid() const noexcept {
    const QVector3D halfExtent = size_ * 0.5f;
    return halfExtent.x() >= convexRadius_ && halfExtent.y() >= convexRadius_ && halfExtent.z() >= convexRadius_;
}

JPH::ShapeRefC BoxCollisionShape::shape() const {
    if (!isValid()) {
        return {};
    }

    const QVector3D halfExtent = size_ * 0.5f;
    const JPH::BoxShapeSettings settings(JPH::Vec3(halfExtent.x(), halfExtent.y(), halfExtent.z()), convexRadius_);
    const auto result = settings.Create();
    return result.IsValid()? result.Get(): JPH::ShapeRefC();
}

VOLCANO_WORLD_END
