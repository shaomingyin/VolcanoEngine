//
//
#include <Volcano/System/SinglePlayer.h>

VOLCANO_SYSTEM_BEGIN

SinglePlayer::SinglePlayer() {
}

Game::World* SinglePlayer::gameWorld() {
	return &game_world_;
}

VOLCANO_SYSTEM_END
