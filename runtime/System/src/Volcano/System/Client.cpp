//
//
#include <Volcano/System/Client.h>

VOLCANO_SYSTEM_BEGIN

Client::Client(QQmlEngine* engine, const QUrl& url, QObject* parent)
    : Local(engine, url, parent) {
}

void Client::update(Duration elapsed) {
    Client::update(elapsed);

    // TODO
}

VOLCANO_SYSTEM_END
