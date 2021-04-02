//
//
#ifndef VOLCANO_DRIVERS_GRPHICS_SPOTLIGHT_HPP
#define VOLCANO_DRIVERS_GRPHICS_SPOTLIGHT_HPP

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Light.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class SpotLight: public Light {
public:
    SpotLight(void);
    virtual ~SpotLight(void);
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRPHICS_MESH_HPP
