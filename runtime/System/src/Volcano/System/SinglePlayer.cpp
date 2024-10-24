//
//
#include <Volcano/System/SinglePlayer.h>

VOLCANO_SYSTEM_BEGIN

SinglePlayer::SinglePlayer()
	: frontend_(resourceManager(), game_world_) {
}

VOLCANO_SYSTEM_END
