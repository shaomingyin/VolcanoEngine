//
//
#include <Volcano/System/MultiPlayerServer.h>

VOLCANO_SYSTEM_BEGIN

MultiPlayerServer::MultiPlayerServer()
	: Base(0) {
}

void MultiPlayerServer::mainLoop() {
	Base::mainLoop();
}

void MultiPlayerServer::frame(Duration elapsed) {
	net_server_.frame(elapsed);
}

VOLCANO_SYSTEM_END
