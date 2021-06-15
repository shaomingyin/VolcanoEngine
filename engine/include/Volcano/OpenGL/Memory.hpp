//
//
#ifndef VOLCANO_OPENGL_MEMORY_HPP
#define VOLCANO_OPENGL_MEMORY_HPP

#include <list>

#include <QIODevice>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Heap.hpp>

VOLCANO_OPENGL_BEGIN

class Memory {
public:
    Memory(void);
    virtual ~Memory(void);

public:
    QIODevice *allocStaticVertices(int count);
    QIODevice *allocStaticVertexIndices(int count);
    QIODevice *allocDynamicVertices(int count);
    QIODevice *allocDynamicVertexIndices(int count);

protected:
    using HeapList = std::list<Heap *>;

    QIODevice *allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    HeapList m_staticVertexHeapList;
    HeapList m_staticVertexIndexHeapList;
    HeapList m_dynamicVertexHeapList;
    HeapList m_dynamicVertexIndexHeapList;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MEMORY_HPP
