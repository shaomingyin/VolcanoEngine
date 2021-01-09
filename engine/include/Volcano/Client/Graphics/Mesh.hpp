//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_MESH_HPP
#define VOLCANO_CLIENT_GRAPHICS_MESH_HPP

#include <Volcano/Client/Graphics/Common.hpp>
#include <Volcano/Client/Graphics/Buffer.hpp>
#include <Volcano/Client/Graphics/Memory.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

class Mesh {
public:
    enum Type {
        TYPE_STATIC = 0,
        TYPE_DYNAMIC
    };

public:
    Mesh(Memory &m);
    virtual ~Mesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount, Type t);
    int vertexCount(void) const;
    int vertexIndexCount(void) const;
    void bind(void);

private:
    Memory &m_memory;
    Buffer *m_vertexBuffer;
    Buffer *m_vertexIndexBuffer;
    int m_vertexCount;
    int m_vertexIndexCount;
};

VOLCANO_INLINE int Mesh::vertexCount(void) const
{
    return m_vertexCount;
}

VOLCANO_INLINE int Mesh::vertexIndexCount(void) const
{
    return m_vertexIndexCount;
}

VOLCANO_INLINE void Mesh::bind(void)
{
    VOLCANO_ASSERT(m_vertexBuffer != nullptr);
    VOLCANO_ASSERT(m_vertexIndexBuffer != nullptr);

    m_vertexBuffer->bind();
    m_vertexIndexBuffer->bind();
}

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_MESH_HPP
