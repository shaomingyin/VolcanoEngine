//
//
#ifndef VOLCANO_GRAPHICS_MEMORY_HPP
#define VOLCANO_GRAPHICS_MEMORY_HPP

#include <list>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>
#include <Volcano/Graphics/Heap.hpp>

VOLCANO_GRAPHICS_BEGIN

class Memory {
public:
    Memory(void);
    virtual ~Memory(void);

public:
    Buffer *allocStaticVertices(int count);
    Buffer *allocStaticVertexIndices(int count);
    Buffer *allocDynamicVertices(int count);
    Buffer *allocDynamicVertexIndices(int count);

private:
    typedef std::list<Heap *> HeapList;

    Buffer *allocBuffer(HeapList &lp, GLsizeiptr size, GLenum target, GLenum usage);

private:
    HeapList m_staticVertexHeaps;
    HeapList m_staticVertexIndexHeaps;
    HeapList m_dynamicVertexHeaps;
    HeapList m_dynamicVertexIndexHeaps;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MEMORY_HPP
