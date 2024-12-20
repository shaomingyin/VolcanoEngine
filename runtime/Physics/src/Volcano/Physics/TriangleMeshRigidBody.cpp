//
//
#include <Volcano/Physics/TriangleMeshRigidBody.h>

VOLCANO_PHYSICS_BEGIN

TriangleMeshRigidBody::TriangleMeshRigidBody(QObject* parent)
    : RigidBody(parent)
    , shape_(new btBvhTriangleMeshShape(nullptr, false)) {
	// btStridingMeshInterface
    setCollisionShape(shape_.get());
}

void TriangleMeshRigidBody::setSource(const QUrl& v) {
    // TODO
    emit sourceChanged(v);
}

VOLCANO_PHYSICS_END
