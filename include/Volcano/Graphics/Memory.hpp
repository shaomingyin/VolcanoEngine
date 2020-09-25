//
//
#ifndef VOLCANO_GRAPHICS_MEMORY_HPP
#define VOLCANO_GRAPHICS_MEMORY_HPP

#include <QList>
#include <QSharedPointer>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Heap.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Memory
{
public:
    Memory(void);
    virtual ~Memory(void);

public:
    Buffer *allocStaticVertexBuffer(int size);
    Buffer *allocStaticVertexIndexBuffer(int size);
    Buffer *allocDynamicVertexBuffer(int size);
    Buffer *allocDynamicVertexIndexBuffer(int size);

private:
    typedef QSharedPointer<Heap> HeapPtr;
    typedef QList<HeapPtr> HeapPtrList;

    Buffer *allocBuffer(int size, HeapPtrList &list,
               QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    HeapPtrList m_staticVertexHeapList;
    HeapPtrList m_staticVertexIndexHeapList;
    HeapPtrList m_dynamicVertexHeapList;
    HeapPtrList m_dynamicVertexIndexHeapList;
};

VOLCANO_INLINE Buffer *Memory::allocStaticVertexBuffer(int size)
{
    return allocBuffer(size, m_staticVertexHeapList,
               QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

VOLCANO_INLINE Buffer *Memory::allocStaticVertexIndexBuffer(int size)
{
    return allocBuffer(size, m_staticVertexIndexHeapList,
               QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

VOLCANO_INLINE Buffer *Memory::allocDynamicVertexBuffer(int size)
{
    return allocBuffer(size, m_dynamicVertexHeapList,
               QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

VOLCANO_INLINE Buffer *Memory::allocDynamicVertexIndexBuffer(int size)
{
    return allocBuffer(size, m_dynamicVertexIndexHeapList,
               QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MEMORY_HPP
