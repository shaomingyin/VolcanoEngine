//
//
#include <Volcano/Framework/Server.h>

VOLCANO_FRAMEWORK_BEGIN

Server::Server(WorldCreator world_creator)
	: Base(std::move(world_creator)) {
}

VOLCANO_FRAMEWORK_END
