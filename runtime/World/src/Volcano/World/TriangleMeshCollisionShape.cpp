//
//
#include <Volcano/World/TriangleMeshCollisionShape.h>

VOLCANO_WORLD_BEGIN

TriangleMeshCollisionShape::TriangleMeshCollisionShape(QObject* parent)
    : CollisionShape(parent) {
}

void TriangleMeshCollisionShape::setSource(const QUrl& v) {
    if (shape_) {
        qmlWarning(this) << "Cannot set the 'source' property after component completed.";
        return;
    }
    if (source_ != v) {
        source_ = v;
        emit sourceChanged(v);
    }
}

void TriangleMeshCollisionShape::componentComplete() {
    // TODO
}

VOLCANO_WORLD_END
