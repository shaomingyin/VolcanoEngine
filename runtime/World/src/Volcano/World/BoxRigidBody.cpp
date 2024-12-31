//
//
#include <Volcano/World/BoxRigidBody.h>

VOLCANO_WORLD_BEGIN

BoxRigidBody::BoxRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btBoxShape({ 1.0f, 1.0f, 1.0f })) {
    setCollisionShape(shape_.get());
}

void BoxRigidBody::resize(float x, float y, float z) {
    bool dx = qFuzzyCompare(size_.x(), x);
    if (dx) {
        size_.setX(x);
    }
    bool dy = qFuzzyCompare(size_.y(), y);
    if (dy) {
        size_.setY(y);
    }
    bool dz = qFuzzyCompare(size_.z(), z);
    if (dz) {
        size_.setZ(z);
    }
    if (dx || dy || dz) {
        preCollisionShapeChange();
        shape_.reset(new btBoxShape({ x, y, z }));
        setCollisionShape(shape_.get());
        postCollisionShapeChange();
        emit sizeChanged(size_);
    }
}

VOLCANO_WORLD_END
