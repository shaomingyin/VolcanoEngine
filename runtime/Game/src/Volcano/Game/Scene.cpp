//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class World : public Object {
public:
	World();
	~World() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_H
