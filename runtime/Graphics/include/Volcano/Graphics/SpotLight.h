//
//
#ifndef VOLCANO_GRAPHICS_SPOTLIGHT_H
#define VOLCANO_GRAPHICS_SPOTLIGHT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/DirectionalLight.h>

VOLCANO_GRAPHICS_BEGIN

class SpotLight: public DirectionalLight {
public:
	SpotLight();
	~SpotLight() override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SPOTLIGHT_H
