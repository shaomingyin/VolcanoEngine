//
//
#include <Volcano/World/BoxCollisionShape.h>

VOLCANO_WORLD_BEGIN

BoxCollisionShape::BoxCollisionShape(QObject* parent)
    : CollisionShape(parent)
    , size_(1.0f, 1.0f, 1.0f) {
}

void BoxCollisionShape::setWidth(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'width' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(size_.z(), v)) {
        size_.setZ(v);
        emit widthChanged(v);
    }
}

void BoxCollisionShape::setHeight(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'height' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(size_.y(), v)) {
        size_.setY(v);
        emit heightChanged(v);
    }
}

void BoxCollisionShape::componentComplete() {
    shape_ = std::make_unique<btBoxShape>(
        btVector3{ size_.x() / 2.0f, size_.y() / 2.0f, size_.z() / 2.0f });
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
