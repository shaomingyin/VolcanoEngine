//
//
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

Object::Object(Context& context)
    : context_(context)
    , entity_(context.registry().create()) {
}

Object::~Object() {
}

VOLCANO_GAME_END
