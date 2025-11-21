//
//
#include <Volcano/Launcher/Client.hpp>

VOLCANO_LAUNCHER_BEGIN

Client::Client(WorldCreator world_creator)
	: Local(std::move(world_creator)) {
}

VOLCANO_LAUNCHER_END
