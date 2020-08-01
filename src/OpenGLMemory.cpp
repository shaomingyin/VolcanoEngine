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

OpenGLBuffer *OpenGLMemory::allocStaticVertexBuffer(int count)
{
    return allocBuffer(m_staticVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(Vertex) * count);
}

OpenGLBuffer *OpenGLMemory::allocStaticVertexIndexBuffer(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(VertexIndex) * count);
}

OpenGLBuffer *OpenGLMemory::allocDynamicVertexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(Vertex) * count);
}

OpenGLBuffer *OpenGLMemory::allocDynamicVertexIndexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(VertexIndex) * count);
}

OpenGLBuffer *OpenGLMemory::allocBuffer(OpenGLHeapList &heapList, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, int size)
{
    OpenGLBuffer *buf;

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
