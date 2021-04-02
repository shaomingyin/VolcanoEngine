//
//
#ifndef VOLCANO_DRIVERS_GRPHICS_MEMORY_HPP
#define VOLCANO_DRIVERS_GRPHICS_MEMORY_HPP

#include <list>

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Heap.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class Memory {
public:
    Memory(void);
    virtual ~Memory(void);

public:
    Buffer *allocStaticVertices(int count);
    Buffer *allocStaticVertexIndices(int count);
    Buffer *allocDynamicVertices(int count);
    Buffer *allocDynamicVertexIndices(int count);

protected:
    using HeapList = std::list<Heap *>;

    Buffer *allocBuffer(HeapList &heapList, GLsizeiptr size, GLenum target, GLenum usage);

private:
    HeapList m_staticVertexHeapList;
    HeapList m_staticVertexIndexHeapList;
    HeapList m_dynamicVertexHeapList;
    HeapList m_dynamicVertexIndexHeapList;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRPHICS_MEMORY_HPP
