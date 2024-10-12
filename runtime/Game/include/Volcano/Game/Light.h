//
//
#ifndef VOLCANO_GAME_LIGHT_H
#define VOLCANO_GAME_LIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Actor.h>

VOLCANO_GAME_BEGIN

class Light: public Actor {
public:
	Light(Context& context);
	~Light() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_H
