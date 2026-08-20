//
//
#include <Volcano/World/TriangleMesh.h>

VOLCANO_WORLD_BEGIN

TriangleMesh::TriangleMesh(QObject* parent)
    : Shape(parent) {
}

const QUrl& TriangleMesh::source() const noexcept {
    return source_;
}

void TriangleMesh::setSource(const QUrl& v) noexcept {
    if (source_ != v) {
        source_ = v;
        emit sourceChanged(v);
    }
}

VOLCANO_WORLD_END
