//
//
#include <Volcano/World/SphereRigidBody.h>

VOLCANO_WORLD_BEGIN

SphereRigidBody::SphereRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(1.0f) {
    setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
