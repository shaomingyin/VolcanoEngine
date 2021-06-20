//
//
#include <memory>

#include <Volcano/OpenGL/Memory.hpp>
#include <Volcano/OpenGL/Mesh.hpp>

VOLCANO_OPENGL_BEGIN

Mesh::Mesh(void):
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

bool Mesh::init(int vertexCount, int vertexIndexCount, bool isDynamic)
{
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);
    Q_ASSERT(vertexCount > 0);
    Q_ASSERT(vertexIndexCount > 0);

    std::unique_ptr<QIODevice> vertexBuffer;
    std::unique_ptr<QIODevice> vertexIndexBuffer;

    if (isDynamic) {
        vertexBuffer.reset(allocDynamicVertices(vertexCount));
        vertexIndexBuffer.reset(allocDynamicVertexIndices(vertexIndexCount));
    } else {
        vertexBuffer.reset(allocStaticVertices(vertexCount));
        vertexIndexBuffer.reset(allocStaticVertexIndices(vertexIndexCount));
    }

    if (!vertexBuffer && !vertexIndexBuffer)
        return false;

    m_vertexBuffer = vertexBuffer.release();
    m_vertexIndexBuffer = vertexIndexBuffer.release();

    return true;
}

VOLCANO_OPENGL_END
