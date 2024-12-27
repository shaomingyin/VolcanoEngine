//
//
#include <Volcano/World/ConeRigidBody.h>

VOLCANO_WORLD_BEGIN

ConeRigidBody::ConeRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(1.0f, 1.0f) {
    setCollisionShape(&shape_);
}

VOLCANO_WORLD_END
