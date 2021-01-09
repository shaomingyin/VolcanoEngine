//
//
#include <Volcano/Client/Graphics/Memory.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

Memory::Memory(void)
{
}

Memory::~Memory(void)
{
}

Buffer *Memory::allocStaticVertices(int count)
{
    return allocBuffer(m_staticVertexHeaps, sizeof(Vertex) * count,
        GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}

Buffer *Memory::allocStaticVertexIndices(int count)
{
    return allocBuffer(m_staticVertexIndexHeaps, sizeof(VertexIndex) * count,
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

Buffer *Memory::allocDynamicVertices(int count)
{
    return allocBuffer(m_dynamicVertexHeaps, sizeof(Vertex) * count,
        GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

Buffer *Memory::allocDynamicVertexIndices(int count)
{
    return allocBuffer(m_dynamicVertexIndexHeaps, sizeof(VertexIndex) * count,
        GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

Buffer *Memory::allocBuffer(HeapList &lp, GLsizeiptr size, GLenum target, GLenum usage)
{
    VOLCANO_ASSERT(size > 0);

    Buffer *buf;

    for (auto it(lp.begin()); it != lp.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    auto ph = new Heap(target, usage);
    if (ph == nullptr)
        return nullptr;

    if (!ph->init()) {
        delete ph;
        return nullptr;
    }

    buf = ph->allocBuffer(size);
    lp.push_front(ph);

    return buf;
}

VOLCANO_CLIENT_GRAPHICS_END
