//
//
#ifndef VOLCANO_GRPHICS_MEMORY_HPP
#define VOLCANO_GRPHICS_MEMORY_HPP

#include <list>

#include <QIODevice>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Heap.hpp>

VOLCANO_GRAPHICS_BEGIN

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

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MEMORY_HPP
