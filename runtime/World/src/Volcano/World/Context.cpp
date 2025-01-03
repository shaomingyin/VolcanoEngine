//
//
#include <Volcano/World/Context.h>

VOLCANO_WORLD_BEGIN

Context::Context(QObject* parent)
    : QObject(parent) {
}

QNetworkAccessManager* Context::networkAccessManager(QObject* object) {
    return qmlEngine(object)->networkAccessManager();
}

VOLCANO_WORLD_END
