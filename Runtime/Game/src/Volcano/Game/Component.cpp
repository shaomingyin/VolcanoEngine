//
//
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

Component::Component(QObject* parent)
    : Actor(parent) {
}

Component::Component(Context& context, QObject* parent)
    : Actor(context, parent) {
}

VOLCANO_GAME_END
