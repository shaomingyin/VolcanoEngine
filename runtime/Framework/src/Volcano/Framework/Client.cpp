//
//
#include <Volcano/Framework/Client.h>

VOLCANO_FRAMEWORK_BEGIN

Client::Client(WorldCreator world_creator)
	: Local(std::move(world_creator)) {
}

VOLCANO_FRAMEWORK_END
