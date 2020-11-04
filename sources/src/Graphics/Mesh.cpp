//
//
#include <QScopedPointer>

#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Memory &memory):
    m_memory(memory),
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr),
    m_vertexCount(0),
    m_vertexIndexCount(0)
{
}

Mesh::~Mesh(void)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    if (m_vertexIndexBuffer != nullptr)
        delete m_vertexIndexBuffer;
}

bool Mesh::init(int vertexCount, int vertexIndexCount, Type type)
{
    Q_ASSERT(vertexCount > 0);
    Q_ASSERT(vertexIndexCount > 0);
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);

    typedef QScopedPointer<Buffer> BufferPtr;

    BufferPtr v;
    BufferPtr vi;

    switch (type)
    {
    case TypeStatic:
        v.reset(m_memory.allocStaticVertexBuffer(sizeof(Vertex) * vertexCount));
        vi.reset(m_memory.allocStaticVertexIndexBuffer(sizeof(VertexIndex) * vertexIndexCount));
        break;
    case TypeDynamic:
        v.reset(m_memory.allocDynamicVertexBuffer(sizeof(Vertex) * vertexCount));
        vi.reset(m_memory.allocDynamicVertexIndexBuffer(sizeof(VertexIndex) * vertexIndexCount));
        break;
    default:
        break;
    }

    if (!v || !vi)
        return false;

    m_vertexBuffer = v.take();
    m_vertexIndexBuffer = vi.take();
    m_vertexCount = vertexCount;
    m_vertexIndexCount = vertexIndexCount;

    return true;
}

VOLCANO_GRAPHICS_END
