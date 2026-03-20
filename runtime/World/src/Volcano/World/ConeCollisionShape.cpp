//
//
#include <Volcano/World/ConeCollisionShape.h>

VOLCANO_WORLD_BEGIN

ConeCollisionShape::ConeCollisionShape(QObject* parent)
    : CollisionShape(parent)
    , radius_(1.0f)
    , height_(1.0f) {
}

void ConeCollisionShape::setRadius(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'radius' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

void ConeCollisionShape::setHeight(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'height' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(height_, v)) {
        height_ = v;
        emit heightChanged(v);
    }
}

void ConeCollisionShape::componentComplete() {
    shape_ = std::make_unique<btConeShape>(radius_, height_);
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
