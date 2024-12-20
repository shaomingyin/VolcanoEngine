//
//
#include <QScopeGuard>

#include <Volcano/Graphics/Mesh.h>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(QObject* parent)
    : World::Transformable(parent) {
}

Mesh::~Mesh() {
}

void Mesh::setSource(const QUrl& v) {
    // TODO
    emit sourceChanged(v);
}

VOLCANO_GRAPHICS_END
