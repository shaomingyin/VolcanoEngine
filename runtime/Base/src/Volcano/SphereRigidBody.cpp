//
//
#include <Volcano/SphereRigidBody.h>

VOLCANO_BEGIN

SphereRigidBody::SphereRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(1.0f) {
    setCollisionShape(&shape_);
}

VOLCANO_END
