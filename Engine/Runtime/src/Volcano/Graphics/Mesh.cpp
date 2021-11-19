//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(QObject *parent):
    Resource(parent)
{
}

Mesh::~Mesh(void)
{
}

void Mesh::onSourceChanged(const QUrl &v)
{
}

VOLCANO_GRAPHICS_END
