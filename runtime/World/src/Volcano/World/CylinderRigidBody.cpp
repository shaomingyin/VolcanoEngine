//
//
#include <Volcano/World/CylinderRigidBody.h>

VOLCANO_WORLD_BEGIN

CylinderRigidBody::CylinderRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btCylinderShape({ 1.0f, 1.0f, 1.0f })) {
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
