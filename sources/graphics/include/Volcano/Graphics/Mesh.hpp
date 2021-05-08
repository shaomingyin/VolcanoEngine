//
//
#ifndef VOLCANO_GRPHICS_MESH_HPP
#define VOLCANO_GRPHICS_MESH_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Memory.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

private:
    Memory &m_memory;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MESH_HPP
