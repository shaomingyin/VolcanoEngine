//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_MESH_HPP
#define VOLCANO_GRAPHICS_OPENGL_MESH_HPP

#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>
#include <Volcano/Graphics/OpenGL/Buffer.hpp>
#include <Volcano/Graphics/OpenGL/Memory.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class VOLCANO_API Mesh: public Graphics::Mesh
{
public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount, Type type = TypeStatic);
    Memory &memory(void);
    Buffer *vertexBuffer(void);
    const Buffer *vertexBuffer(void) const;
    Buffer *vertexIndexBuffer(void);
    const Buffer *vertexIndexBuffer(void) const;
    int vertexCount(void) const;
    int vertexIndexCount(void) const;

private:
    Memory &m_memory;
    Buffer *m_vertexBuffer;
    Buffer *m_vertexIndexBuffer;
    int m_vertexCount;
    int m_vertexIndexCount;
};

VOLCANO_INLINE Memory &Mesh::memory(void)
{
    return m_memory;
}

VOLCANO_INLINE Buffer *Mesh::vertexBuffer(void)
{
    return m_vertexBuffer;
}

VOLCANO_INLINE const Buffer *Mesh::vertexBuffer(void) const
{
    return m_vertexBuffer;
}

VOLCANO_INLINE Buffer *Mesh::vertexIndexBuffer(void)
{
    return m_vertexIndexBuffer;
}

VOLCANO_INLINE const Buffer *Mesh::vertexIndexBuffer(void) const
{
    return m_vertexIndexBuffer;
}

VOLCANO_INLINE int Mesh::vertexCount(void) const
{
    return m_vertexCount;
}

VOLCANO_INLINE int Mesh::vertexIndexCount(void) const
{
    return m_vertexIndexCount;
}

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_MESH_HPP
