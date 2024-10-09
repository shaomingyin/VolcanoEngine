//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_H
#define VOLCANO_GAME_SPOTLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

class SpotLight: public Light {
public:
	SpotLight();
	~SpotLight() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_H
