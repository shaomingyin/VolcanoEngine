//
//
#include <QtQml>

#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

Object::Object(QObject* parent)
    : QObject(parent) {
}

QNetworkAccessManager* Object::networkAccessManager() {
    auto qml_engine = qmlEngine(this);
    if (qml_engine != nullptr) {
        return qml_engine->networkAccessManager();
    }
    return nullptr;
}

VOLCANO_WORLD_END
