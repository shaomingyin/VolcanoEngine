//
//
#include <Volcano/Game/Mesh.hpp>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
