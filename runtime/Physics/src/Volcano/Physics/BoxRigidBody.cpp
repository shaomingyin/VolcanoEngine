//
//
#include <Volcano/Physics/BoxRigidBody.h>

VOLCANO_PHYSICS_BEGIN

BoxRigidBody::BoxRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btBoxShape({ 1.0f, 1.0f, 1.0f })) {
    setCollisionShape(shape_.get());
}

void BoxRigidBody::resize(const QVector3D& v) {
    preCollisionShapeChange();
    shape_.reset(new btBoxShape({ v.x(), v.y(), v.z() }));
    setCollisionShape(shape_.get());
    postCollisionShapeChange();
    emit sizeChanged(v);
}

VOLCANO_PHYSICS_END
