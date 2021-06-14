//
//
#include <memory>

#include <Volcano/Graphics/Memory.hpp>

VOLCANO_GRAPHICS_BEGIN

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

QIODevice *Memory::allocStaticVertices(int count)
{
    return allocBuffer(m_staticVertexHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

QIODevice *Memory::allocStaticVertexIndices(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

QIODevice *Memory::allocDynamicVertices(int count)
{
    return allocBuffer(m_dynamicVertexHeapList, sizeof(Vertex) * count,
       QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

QIODevice *Memory::allocDynamicVertexIndices(int count)
{
    return allocBuffer(m_dynamicVertexIndexHeapList, sizeof(VertexIndex) * count,
       QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

QIODevice *Memory::allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    QIODevice *buf;

    for (auto it(heapList.begin()); it != heapList.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    auto heap = std::make_unique<Heap>(type, usage);
    if (!heap || !heap->init())
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.push_front(heap.release());

    return buf;
}

VOLCANO_GRAPHICS_END
