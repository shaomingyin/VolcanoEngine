//
//
#include <Volcano/Actor.hpp>

VOLCANO_BEGIN

Actor::Actor(QObject *parent)
    : Object(parent) {
}

Actor::Actor(Context& context, QObject *parent)
    : Object(context, parent) {
}

VOLCANO_END
