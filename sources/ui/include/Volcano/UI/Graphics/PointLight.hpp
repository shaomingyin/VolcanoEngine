//
//
#ifndef VOLCANO_UI_GRPHICS_POINTLIGHT_HPP
#define VOLCANO_UI_GRPHICS_POINTLIGHT_HPP

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Light.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class PointLight: public Light {
public:
    PointLight(void);
    ~PointLight(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_POINTLIGHT_HPP
