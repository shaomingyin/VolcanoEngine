//
//
#include <Volcano/System/Server.h>

VOLCANO_SYSTEM_BEGIN

Server::Server(QQmlEngine* engine, QObject* parent)
    : Base(engine, parent) {
}

void Server::update(Duration elapsed) {
    Base::update(elapsed);

    // TODO
}

VOLCANO_SYSTEM_END
