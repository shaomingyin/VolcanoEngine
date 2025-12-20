//
//
#include <Volcano/Game/CapsuleRigidBody.h>

VOLCANO_GAME_BEGIN

CapsuleRigidBody::CapsuleRigidBody(QObject* parent)
    : RigidBody(parent)
    , radius_(1.0f)
    , height_(1.0f) {
}

void CapsuleRigidBody::setRadius(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'radius' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

void CapsuleRigidBody::setHeight(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'height' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(height_, v)) {
        height_ = v;
        emit heightChanged(v);
    }
}

void CapsuleRigidBody::componentComplete() {
    shape_ = std::make_unique<btCapsuleShape>(radius_, height_);
    setCollisionShape(shape_.get());
}

VOLCANO_GAME_END
