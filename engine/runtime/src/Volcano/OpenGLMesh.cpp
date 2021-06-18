//
//
#include <memory>

#include <Volcano/OpenGLMemory.hpp>
#include <Volcano/OpenGLMesh.hpp>

VOLCANO_BEGIN

OpenGLMesh::OpenGLMesh(void):
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr)
{
}

OpenGLMesh::~OpenGLMesh(void)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    if (m_vertexIndexBuffer != nullptr)
        delete m_vertexIndexBuffer;
}

bool OpenGLMesh::init(int vertexCount, int vertexIndexCount, bool isDynamic)
{
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);
    Q_ASSERT(vertexCount > 0);
    Q_ASSERT(vertexIndexCount > 0);

    std::unique_ptr<QIODevice> vertexBuffer;
    std::unique_ptr<QIODevice> vertexIndexBuffer;

    if (isDynamic) {
        vertexBuffer.reset(glAllocDynamicVertices(vertexCount));
        vertexIndexBuffer.reset(glAllocDynamicVertexIndices(vertexIndexCount));
    } else {
        vertexBuffer.reset(glAllocStaticVertices(vertexCount));
        vertexIndexBuffer.reset(glAllocStaticVertexIndices(vertexIndexCount));
    }

    if (!vertexBuffer && !vertexIndexBuffer)
        return false;

    m_vertexBuffer = vertexBuffer.release();
    m_vertexIndexBuffer = vertexIndexBuffer.release();

    return true;
}

VOLCANO_END
