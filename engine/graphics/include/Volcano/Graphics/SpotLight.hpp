//
//
#ifndef VOLCANO_GRAPHICS_SPOTLIGHT_HPP
#define VOLCANO_GRAPHICS_SPOTLIGHT_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class SpotLight: public Light {
public:
    SpotLight(void);
    virtual ~SpotLight(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MESH_HPP
