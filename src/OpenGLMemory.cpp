//
//
#include <Volcano/OpenGLMemory.hpp>

VOLCANO_BEGIN

OpenGLMemory::OpenGLMemory(void)
{
}

OpenGLMemory::~OpenGLMemory(void)
{
}

bool OpenGLMemory::init(void)
{
    return true;
}

QIODevice *OpenGLMemory::allocStaticVertexBuffer(int count)
{
    return allocBuffer(m_staticVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(Vertex) * count);
}

QIODevice *OpenGLMemory::allocStaticVertexIndexBuffer(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(VertexIndex) * count);
}

QIODevice *OpenGLMemory::allocDynamicVertexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(Vertex) * count);
}

QIODevice *OpenGLMemory::allocDynamicVertexIndexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(VertexIndex) * count);
}

QIODevice *OpenGLMemory::allocBuffer(OpenGLHeapList &heapList, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, int size)
{
    QIODevice *buf;

    for (auto it(heapList.begin()); it != heapList.end(); ++it)
    {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    OpenGLHeap *heap = new OpenGLHeap(type, usage);
    if (heap == nullptr)
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.prepend(heap);

    return buf;
}

VOLCANO_END
