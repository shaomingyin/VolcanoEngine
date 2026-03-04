//
//
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

Light::Light(QObject* parent)
    : Transformable(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

VOLCANO_GAME_END
