//
//
#include <Volcano/Server/Application.hpp>

VOLCANO_SERVER_BEGIN

Server::Server(WorldCreator world_creator)
	: Base(std::move(world_creator)) {
}

VOLCANO_SERVER_END
