//
//
#ifndef VOLCANO_DRIVERS_GRPHICS_POINTLIGHT_HPP
#define VOLCANO_DRIVERS_GRPHICS_POINTLIGHT_HPP

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Light.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class PointLight: public Light {
public:
    PointLight(void);
    ~PointLight(void) override;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRPHICS_POINTLIGHT_HPP
