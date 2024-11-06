//
//
#ifndef VOLCANO_GRAPHICS_POINTLIGHT_H
#define VOLCANO_GRAPHICS_POINTLIGHT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

class PointLight: public Light {
public:
	PointLight();
	~PointLight() override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_POINTLIGHT_H
