//
//
#include <Volcano/Graphics/DirectionalLight.h>

VOLCANO_GRAPHICS_BEGIN

DirectionalLight::DirectionalLight(QObject* parent)
    : Light(parent)
    , direction_(0.0f, -1.0f, 0.0f) {
}

VOLCANO_GRAPHICS_END
