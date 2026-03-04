//
//
#include <Volcano/Game/SphereRigidBody.h>

VOLCANO_GAME_BEGIN

SphereRigidBody::SphereRigidBody(QObject* parent)
    : RigidBody(parent)
    , radius_(1.0f) {
}

void SphereRigidBody::setRadius(float v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'radius' property after component completed.";
        return;
    }
    if (!qFuzzyCompare(radius_, v)) {
        radius_ = v;
        emit radiusChanged(v);
    }
}

void SphereRigidBody::componentComplete() {
    shape_ = std::make_unique<btSphereShape>(radius_);
    setCollisionShape(shape_.get());
}

VOLCANO_GAME_END
