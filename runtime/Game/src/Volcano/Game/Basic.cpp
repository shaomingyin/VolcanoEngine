//
//
#include <Volcano/Game/Basic.h>

VOLCANO_GAME_BEGIN

Basic::Basic(std::string name, int flags)
	: flags_(flags)
	, name_(std::move(name)) {
}

VOLCANO_GAME_END
