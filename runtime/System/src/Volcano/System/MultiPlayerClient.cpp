//
//
#include <Volcano/System/MultiPlayerClient.h>

VOLCANO_SYSTEM_BEGIN

MultiPlayerClient::MultiPlayerClient() {
}

Game::World* MultiPlayerClient::gameWorld() {
	return &net_client_;
}

VOLCANO_SYSTEM_END
