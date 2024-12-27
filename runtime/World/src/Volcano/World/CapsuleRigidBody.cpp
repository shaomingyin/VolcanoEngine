//
//
#include <Volcano/World/CapsuleRigidBody.h>

VOLCANO_WORLD_BEGIN

CapsuleRigidBody::CapsuleRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btCapsuleShape(1.0f, 1.0f)) {
    setCollisionShape(shape_.get());
}

VOLCANO_WORLD_END
