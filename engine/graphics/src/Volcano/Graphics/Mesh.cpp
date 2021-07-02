//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Game::Mesh *gameMesh):
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

const Game::Mesh *Mesh::gameMesh(void) const
{
    return m_gameMesh;
}

bool Mesh::init(void)
{
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);

    return true;
}

VOLCANO_GRAPHICS_END
