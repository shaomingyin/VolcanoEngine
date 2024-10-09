//
//
#ifndef VOLCANO_GAME_ENTITY_H
#define VOLCANO_GAME_ENTITY_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Actor.h>

VOLCANO_GAME_BEGIN

class Entity: public Actor {
public:
	Entity();
	~Entity() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_H
