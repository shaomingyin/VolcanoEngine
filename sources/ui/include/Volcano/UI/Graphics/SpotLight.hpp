//
//
#ifndef VOLCANO_UI_GRPHICS_SPOTLIGHT_HPP
#define VOLCANO_UI_GRPHICS_SPOTLIGHT_HPP

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Light.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class SpotLight: public Light {
public:
    SpotLight(void);
    virtual ~SpotLight(void);
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MESH_HPP
