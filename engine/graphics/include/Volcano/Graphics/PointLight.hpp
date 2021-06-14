//
//
#ifndef VOLCANO_GRAPHICS_POINTLIGHT_HPP
#define VOLCANO_GRAPHICS_POINTLIGHT_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class PointLight: public Light {
public:
    PointLight(void);
    ~PointLight(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_POINTLIGHT_HPP
