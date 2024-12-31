//
//
#include <Volcano/World/CapsuleRigidBody.h>

VOLCANO_WORLD_BEGIN

CapsuleRigidBody::CapsuleRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btCapsuleShape(1.0f, 1.0f)) {
    setCollisionShape(shape_.get());
}

void CapsuleRigidBody::setRadius(float v) {
    float h = height();
    shape_.reset(new btCapsuleShape({ v, h }));
    setCollisionShape(shape_.get());
    emit radiusChanged(v);
}

void CapsuleRigidBody::setHeight(float v) {
    float r = shape_->getRadius();
    shape_.reset(new btCapsuleShape({ r, v }));
    setCollisionShape(shape_.get());
    emit heightChanged(v);
}

VOLCANO_WORLD_END
