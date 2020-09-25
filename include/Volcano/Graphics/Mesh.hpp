//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Mesh
{
public:
    enum Type
    {
        TypeStatic = 0,
        TypeDynamic
    };

public:
    Mesh(void);
    virtual ~Mesh(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_HPP
