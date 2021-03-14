//
//
#ifndef VOLCANO_UI_GRPHICS_MESH_HPP
#define VOLCANO_UI_GRPHICS_MESH_HPP

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Memory.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

private:
    Memory &m_memory;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MESH_HPP
