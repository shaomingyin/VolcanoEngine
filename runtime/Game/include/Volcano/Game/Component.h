//
//
#ifndef VOLCANO_GAME_COMPONENT_H
#define VOLCANO_GAME_COMPONENT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Component: public Object {
public:
	Component();
	~Component() override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_H
