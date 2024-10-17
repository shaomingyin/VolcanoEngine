//
//
#include <Volcano/Game/Entity.h>

VOLCANO_GAME_BEGIN

Entity::Entity(Context& context)
    : Actor(context)
    , id_(context.registry().create()) {
}

Entity::~Entity() {
    context().registry().destroy(id_);
}

VOLCANO_GAME_END
