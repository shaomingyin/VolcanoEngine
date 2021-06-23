//
//
#include <memory>

#include <Volcano/Game/Graphics/Memory.hpp>
#include <Volcano/Game/Graphics/Mesh.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Mesh::Mesh(QObject *parent):
    Resource(parent),
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

void Mesh::load(void)
{
#if 0 // TODO should be in rendering thread.
    if (m_vertexBuffer != nullptr) {
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }

    if (m_vertexIndexBuffer != nullptr) {
        delete m_vertexIndexBuffer;
        m_vertexIndexBuffer = nullptr;
    }

    std::unique_ptr<QIODevice> vertexBuffer;
    std::unique_ptr<QIODevice> vertexIndexBuffer;
#endif
}

VOLCANO_GAME_GRAPHICS_END
