//
//
#include <Volcano/World/AmbientLight.h>

VOLCANO_WORLD_BEGIN

AmbientLight::AmbientLight(QObject* parent)
    : Object(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

VOLCANO_WORLD_END
