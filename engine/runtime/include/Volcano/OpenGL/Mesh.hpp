//
//
#ifndef VOLCANO_OPENGL_MESH_HPP
#define VOLCANO_OPENGL_MESH_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Memory.hpp>

VOLCANO_OPENGL_BEGIN

class Mesh {
public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

private:
    Memory &m_memory;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MESH_HPP
