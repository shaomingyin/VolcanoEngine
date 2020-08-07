//
//
#ifndef VOLCANO_GRAPHICS_MEMORY_HPP
#define VOLCANO_GRAPHICS_MEMORY_HPP

#include <QList>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>
#include <Volcano/Graphics/Heap.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Memory
{
public:
    Memory(void);
    virtual ~Memory(void);

public:
    bool init(void);
    Buffer *allocStaticVertexBuffer(int count);
    Buffer *allocStaticVertexIndexBuffer(int count);
    Buffer *allocDynamicVertexBuffer(int count);
    Buffer *allocDynamicVertexIndexBuffer(int count);

private:
    typedef QList<Heap *> HeapList;
    Buffer *allocBuffer(HeapList &heapList, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, int size);

private:
    HeapList m_staticVertexHeapList;
    HeapList m_staticVertexIndexHeapList;
    HeapList m_dynamicVertexHeapList;
    HeapList m_dynamicVertexIndexHeapList;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MEMORY_HPP
