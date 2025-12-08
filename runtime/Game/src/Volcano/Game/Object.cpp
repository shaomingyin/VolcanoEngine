//
//
#include <QtQml>

#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

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

QDataStream& operator<<(QDataStream& s, const Object& v) {
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Object& v) {
    // TODO
    return s;
}

VOLCANO_GAME_END
