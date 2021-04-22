//
//
#ifndef VOLCANO_GRPHICS_MESH_HPP
#define VOLCANO_GRPHICS_MESH_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(void);
    virtual ~Mesh(void);

private:
    GLEXMesh *m_mesh;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MESH_HPP
