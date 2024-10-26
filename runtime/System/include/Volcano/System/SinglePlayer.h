//
//
#ifndef VOLCANO_SYSTEM_SINGLEPLAYER_H
#define VOLCANO_SYSTEM_SINGLEPLAYER_H

#include <Volcano/Game/World.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/FrontEnd.h>
#include <Volcano/System/LocalPlayer.h>

VOLCANO_SYSTEM_BEGIN

class SinglePlayer: public LocalPlayer {
public:
	SinglePlayer();
	virtual ~SinglePlayer() = default;

protected:
	Game::World* gameWorld() override;

private:
	Game::World game_world_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SINGLEPLAYER_H
