//
//
#include <Volcano/System/Client.h>

VOLCANO_SYSTEM_BEGIN

Client::Client(World::Scene& scene, QObject* parent)
    : Local(scene, parent) {
}

void Client::update(Duration elapsed) {
    Local::update(elapsed);

    // TODO
}

VOLCANO_SYSTEM_END
