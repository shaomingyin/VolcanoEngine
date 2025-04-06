//
//
#ifndef VOLCANO_GAME_ACTOR_H
#define VOLCANO_GAME_ACTOR_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Actor: public Object {
public:
	Actor();
	~Actor() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTOR_H
