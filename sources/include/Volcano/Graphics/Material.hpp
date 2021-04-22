//
//
#ifndef VOLCANO_GRPHICS_MATERIAL_HPP
#define VOLCANO_GRPHICS_MATERIAL_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Material {
public:
    Material(void);
    virtual ~Material(void);

private:
    GLEXMaterial *m_material;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MATERIAL_HPP
