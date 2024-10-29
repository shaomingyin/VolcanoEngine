//
//
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

Light::Light()
    : color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

Light::~Light() {
}

VOLCANO_WORLD_END
