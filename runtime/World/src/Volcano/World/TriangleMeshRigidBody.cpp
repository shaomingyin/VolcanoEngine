//
//
#include <Volcano/World/TriangleMeshRigidBody.h>

VOLCANO_WORLD_BEGIN

TriangleMeshRigidBody::TriangleMeshRigidBody(QObject* parent)
    : RigidBody(parent) {
}

void TriangleMeshRigidBody::setSource(const QUrl& v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'source' property after component completed.";
        return;
    }
    if (source_ != v) {
        source_ = v;
        emit sourceChanged(v);
    }
}

void TriangleMeshRigidBody::componentComplete() {
    // TODO
}

VOLCANO_WORLD_END
