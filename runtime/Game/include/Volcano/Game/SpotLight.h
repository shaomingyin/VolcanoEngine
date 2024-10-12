//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_H
#define VOLCANO_GAME_SPOTLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/DirectionalLight.h>

VOLCANO_GAME_BEGIN

class SpotLight: public DirectionalLight {
public:
	SpotLight(Context& context);
	~SpotLight() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_H
