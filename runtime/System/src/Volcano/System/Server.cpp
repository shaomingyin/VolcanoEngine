//
//
#include <Volcano/System/Server.h>

VOLCANO_SYSTEM_BEGIN

Server::Server(World::Scene& scene, QObject* parent)
    : Base(scene, parent) {
}

void Server::update(Duration elapsed) {
    Base::update(elapsed);

    // TODO
}

VOLCANO_SYSTEM_END
