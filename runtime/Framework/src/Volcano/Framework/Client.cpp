#include <Volcano/Framework/Client.h>

VOLCANO_FRAMEWORK_BEGIN

Client::~Client() = default;

void Client::onBeforeUpdate(Clock::duration elapsed) {
    Local::onBeforeUpdate(elapsed);
    onNetworkUpdate(elapsed);
}

void Client::onNetworkUpdate(Clock::duration) {
}

VOLCANO_FRAMEWORK_END
