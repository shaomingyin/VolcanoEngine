//
//
#ifndef VOLCANO_OPENGL_MESH_HPP
#define VOLCANO_OPENGL_MESH_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Buffer.hpp>
#include <Volcano/OpenGL/Memory.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Mesh
{
public:
    enum Type
    {
        TypeStatic = 0,
        TypeDynamic
    };

public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

public:
    bool init(int vertexCount, int vertexIndexCount, Type type = TypeStatic);
    Buffer *vertexBuffer(void);
    Buffer *vertexIndexBuffer(void);
    int vertexCount(void) const;
    int vertexIndexCount(void) const;

private:
    Memory &m_memory;
    Buffer *m_vertexBuffer;
    Buffer *m_vertexIndexBuffer;
    int m_vertexCount;
    int m_vertexIndexCount;
};

VOLCANO_INLINE Buffer *Mesh::vertexBuffer(void)
{
    return m_vertexBuffer;
}

VOLCANO_INLINE Buffer *Mesh::vertexIndexBuffer(void)
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

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MESH_HPP
