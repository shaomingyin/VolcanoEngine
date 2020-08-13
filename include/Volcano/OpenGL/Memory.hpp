//
//
#ifndef VOLCANO_OPENGL_MEMORY_HPP
#define VOLCANO_OPENGL_MEMORY_HPP

#include <QList>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Buffer.hpp>
#include <Volcano/OpenGL/Heap.hpp>

VOLCANO_OPENGL_BEGIN

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

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MEMORY_HPP
