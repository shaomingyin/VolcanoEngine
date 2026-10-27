#include <Volcano/Framework/Server.h>

VOLCANO_FRAMEWORK_BEGIN

Server::Server() = default;
Server::~Server() = default;

void Server::onBeforeUpdate(Clock::duration elapsed) {
    Base::onBeforeUpdate(elapsed);
    onNetworkUpdate(elapsed);
}

void Server::onNetworkUpdate(Clock::duration) {
}

VOLCANO_FRAMEWORK_END
