//
//
#ifndef VOLCANO_DRIVERS_GRPHICS_MESH_HPP
#define VOLCANO_DRIVERS_GRPHICS_MESH_HPP

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Memory.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(Memory &memory);
    virtual ~Mesh(void);

private:
    Memory &m_memory;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRPHICS_MESH_HPP
