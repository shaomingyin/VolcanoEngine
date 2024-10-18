//
//
#ifndef VOLCANO_GAME_POINTLIGHT_H
#define VOLCANO_GAME_POINTLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

class PointLight: public Light {
public:
	PointLight();
	~PointLight() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_POINTLIGHT_H
