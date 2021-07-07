//
//
#include <QThread>

#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Game::Mesh *gameMesh, QObject *parent):
    Renderable(parent),
    m_gameMesh(gameMesh),
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr)
{
}

Mesh::~Mesh(void)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    if (m_vertexIndexBuffer != nullptr)
        delete m_vertexIndexBuffer;
}

VOLCANO_GRAPHICS_END
