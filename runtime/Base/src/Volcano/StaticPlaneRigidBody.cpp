//
//
#include <Volcano/StaticPlaneRigidBody.h>

VOLCANO_BEGIN

StaticPlaneRigidBody::StaticPlaneRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btStaticPlaneShape({ 0.0f, 1.0f, 0.0f }, 0.0f)) {
    setCollisionShape(shape_.get());
	//setFlags()STATIC;
}

VOLCANO_END
