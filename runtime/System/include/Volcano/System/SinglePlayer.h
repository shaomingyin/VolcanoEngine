//
//
#ifndef VOLCANO_SYSTEM_SINGLEPLAYER_H
#define VOLCANO_SYSTEM_SINGLEPLAYER_H

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/FrontEnd.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class SinglePlayer: public Base {
public:
	SinglePlayer();
	virtual ~SinglePlayer() = default;

private:
	Game::World game_world_;
	Frontend frontend_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SINGLEPLAYER_H
