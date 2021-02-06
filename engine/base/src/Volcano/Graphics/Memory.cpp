//
//
#include <QScopedPointer>

#include <Volcano/Graphics/Memory.hpp>

VOLCANO_GRAPHICS_BEGIN

Memory::Memory(QObject *parent):
    QObject(parent)
{
}

Memory::~Memory(void)
{
}

Buffer *Memory::allocStaticVertices(int count)
{
    return allocBuffer(m_staticVertexHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *Memory::allocStaticVertexIndices(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *Memory::allocDynamicVertices(int count)
{
    return allocBuffer(m_dynamicVertexHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

Buffer *Memory::allocDynamicVertexIndices(int count)
{
    return allocBuffer(m_dynamicVertexIndexHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

Buffer *Memory::allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type  type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    Buffer *buf;

    for (auto it(heapList.begin()); it != heapList.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    QScopedPointer<Heap> heap(new Heap(type, usage));
    if (!heap || !heap->init())
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.push_front(heap.take());

    return buf;
}

VOLCANO_GRAPHICS_END
