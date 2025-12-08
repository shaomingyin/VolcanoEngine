//
//
#include <Volcano/Game/TriangleMeshRigidBody.h>

VOLCANO_GAME_BEGIN

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

QDataStream& operator<<(QDataStream& s, const TriangleMeshRigidBody& v) {
    s << static_cast<const RigidBody&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, TriangleMeshRigidBody& v) {
    s >> static_cast<RigidBody&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
