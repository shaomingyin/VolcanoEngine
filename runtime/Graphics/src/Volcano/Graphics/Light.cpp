//
//
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

Light::Light()
    : color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

Light::~Light() {
}

VOLCANO_GRAPHICS_END
