//
//
#include <Volcano/Game/AmbientLight.h>

VOLCANO_GAME_BEGIN

AmbientLight::AmbientLight(QObject* parent)
    : Object(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

VOLCANO_GAME_END
