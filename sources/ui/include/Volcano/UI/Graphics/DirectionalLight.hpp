//
//
#ifndef VOLCANO_UI_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_UI_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Light.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRAPHICS_DIRECTIONALLIGHT_HPP
