//
//
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

Light::Light(QObject* parent)
    : World::Transformable(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

VOLCANO_GRAPHICS_END
