//
//
#include <Volcano/Physics/ConeRigidBody.h>

VOLCANO_PHYSICS_BEGIN

ConeRigidBody::ConeRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(1.0f, 1.0f) {
    setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
