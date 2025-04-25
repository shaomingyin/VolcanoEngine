//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class World {
public:
	World();
	virtual ~World();

public:
	virtual void update(Duration elapsed) = 0;

private:
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
