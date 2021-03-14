//
//
#include <memory>

#include <Volcano/UI/Graphics/Memory.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

Buffer *Memory::allocStaticVertices(int count)
{
    return allocBuffer(m_staticVertexHeapList, sizeof(Vertex) * count, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}

Buffer *Memory::allocStaticVertexIndices(int count)
{
    return allocBuffer(m_staticVertexIndexHeapList, sizeof(VertexIndex) * count, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

Buffer *Memory::allocDynamicVertices(int count)
{
    return allocBuffer(m_dynamicVertexHeapList, sizeof(Vertex) * count, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

Buffer *Memory::allocDynamicVertexIndices(int count)
{
    return allocBuffer(m_dynamicVertexIndexHeapList, sizeof(VertexIndex) * count, GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

Buffer *Memory::allocBuffer(HeapList &heapList, GLsizeiptr size, GLenum target, GLenum usage)
{
    VOLCANO_ASSERT(size > 0);

    Buffer *buf;

    for (auto it(heapList.begin()); it != heapList.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    auto heap = std::make_unique<Heap>(target, usage);
    if (!heap || !heap->init())
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.push_front(heap.release());

    return buf;
}

VOLCANO_UI_GRAPHICS_END
