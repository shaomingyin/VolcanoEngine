//
//
#include <Volcano/Graphics/Memory.hpp>

VOLCANO_GRAPHICS_BEGIN

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

bool Memory::init(void)
{
    return true;
}

Buffer *Memory::allocStaticVertexBuffer(int count)
{
    return allocBuffer(m_staticVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(Vertex) * count);
}

Buffer *Memory::allocStaticVertexIndexBuffer(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw,
        sizeof(VertexIndex) * count);
}

Buffer *Memory::allocDynamicVertexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(Vertex) * count);
}

Buffer *Memory::allocDynamicVertexIndexBuffer(int count)
{
    return allocBuffer(m_dynamicVertexHeapList,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw,
        sizeof(VertexIndex) * count);
}

Buffer *Memory::allocBuffer(HeapList &heapList, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, int size)
{
    Buffer *buf;

    for (auto it(heapList.begin()); it != heapList.end(); ++it)
    {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    Heap *heap = new Heap(type, usage);
    if (heap == nullptr)
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.prepend(heap);

    return buf;
}

VOLCANO_GRAPHICS_END
