//
//
#include <Volcano/OpenGLMesh.hpp>

VOLCANO_BEGIN

OpenGLMesh::OpenGLMesh(OpenGLMemory &memory):
    m_memory(memory),
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

bool OpenGLMesh::init(int vertexCount, int vertexIndexCount)
{
    Q_ASSERT(vertexCount > 0);
    Q_ASSERT(vertexIndexCount > 0);
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);

    m_vertexBuffer = m_memory.allocStaticVertexBuffer(vertexCount);
    if (m_vertexBuffer == nullptr)
        return false;

    m_vertexIndexBuffer = m_memory.allocStaticVertexIndexBuffer(vertexIndexCount);
    if (m_vertexIndexBuffer == nullptr) {
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
        return false;
    }

    return true;
}

VOLCANO_END
