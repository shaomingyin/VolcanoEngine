//
//
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

Light::Light(QObject* parent)
    : Transformable(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

VOLCANO_WORLD_END
