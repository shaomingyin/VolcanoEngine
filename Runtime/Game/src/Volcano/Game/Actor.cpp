//
//
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

Actor::Actor(QObject *parent)
    : Object(parent) {
}

Actor::Actor(Context& context, QObject *parent)
    : Object(context, parent) {
}

VOLCANO_GAME_END
