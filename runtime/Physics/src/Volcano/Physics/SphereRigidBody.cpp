//
//
#include <Volcano/Physics/SphereRigidBody.h>

VOLCANO_PHYSICS_BEGIN

SphereRigidBody::SphereRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(1.0f) {
    setCollisionShape(&shape_);
}

VOLCANO_PHYSICS_END
