//
//
#include <Volcano/World/CapsuleCollisionShape.h>

VOLCANO_WORLD_BEGIN

CapsuleCollisionShape::CapsuleCollisionShape(QObject* parent)
    : CollisionShape(parent)
    , radius_(1.0f)
    , height_(1.0f) {
}

void CapsuleCollisionShape::setRadius(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'radius' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

void CapsuleCollisionShape::setHeight(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'height' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(height_, v)) {
        height_ = v;
        emit heightChanged(v);
    }
}

void CapsuleCollisionShape::componentComplete() {
    shape_ = std::make_unique<btCapsuleShape>(radius_, height_);
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
