//
//
#include <Volcano/Game/Component.h>

VOLCANO_GAME_BEGIN

Component::Component(QObject* parent)
    : Object(parent) {
}

QDataStream& operator<<(QDataStream& s, const Component& v) {
    s << static_cast<const Object&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Component& v) {
    s >> static_cast<Object&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
