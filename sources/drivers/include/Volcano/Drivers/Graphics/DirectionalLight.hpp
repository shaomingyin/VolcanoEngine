//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_DRIVERS_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Light.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_DIRECTIONALLIGHT_HPP
