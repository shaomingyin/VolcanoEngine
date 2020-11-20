//
//
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Memory &m):
    m_memory(m),
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr),
    m_vertexCount(0),
    m_vertexIndexCount(0)
{
}

Mesh::~Mesh(void)
{
}

bool Mesh::init(int vertexCount, int vertexIndexCount, Type type)
{
    VOLCANO_ASSERT(m_vertexBuffer == nullptr);
    VOLCANO_ASSERT(m_vertexIndexBuffer == nullptr);

    switch (type) {
    case TYPE_STATIC:
        m_vertexBuffer = m_memory.allocStaticVertices(vertexCount);
        if (m_vertexBuffer == nullptr)
            return false;
        m_vertexIndexBuffer = m_memory.allocStaticVertexIndices(vertexIndexCount);
        if (m_vertexIndexBuffer == nullptr) {
            delete m_vertexBuffer;
            return false;
        }
        break;
    case TYPE_DYNAMIC:
        m_vertexBuffer = m_memory.allocDynamicVertices(vertexCount);
        if (m_vertexBuffer == nullptr)
            return false;
        m_vertexIndexBuffer = m_memory.allocDynamicVertexIndices(vertexIndexCount);
        if (m_vertexIndexBuffer == nullptr) {
            delete m_vertexBuffer;
            return false;
        }
        break;
    default:
        return false;
    }

    m_vertexCount = vertexCount;
    m_vertexIndexCount = vertexIndexCount;

    return true;
}

VOLCANO_GRAPHICS_END
