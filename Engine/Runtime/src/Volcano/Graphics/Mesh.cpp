//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Game::Mesh *p, QObject *parent):
    QObject(parent),
    m_gameMesh(p)
{
    Q_ASSERT(p != nullptr);
    connect(p, &Game::Mesh::sourceChanged, this, &Mesh::onSourceChanged);
    onSourceChanged(p->source());
}

Mesh::~Mesh(void)
{
}

Game::Mesh *Mesh::gameMesh(void)
{
    return m_gameMesh;
}

void Mesh::onSourceChanged(const QUrl &v)
{
    // TODO post a init task to renderer thread.
}

VOLCANO_GRAPHICS_END
