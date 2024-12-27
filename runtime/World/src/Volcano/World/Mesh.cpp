//
//
#include <QScopeGuard>

#include <Volcano/World/Mesh.h>

VOLCANO_WORLD_BEGIN

Mesh::Mesh(QObject* parent)
    : Transformable(parent) {
}

Mesh::~Mesh() {
}

VOLCANO_WORLD_END
