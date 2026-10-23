//
//
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

Light::Light() noexcept
    : color_(1.0f, 1.0f, 1.0f)
    , strength_(1.0f) {
}

Light::Light(const Volcano::Math::Vector3f& color, float strength) noexcept
    : color_(color)
    , strength_(strength) {
}

Light::Light(float r, float g, float b, float strength) noexcept
    : Light(Volcano::Math::Vector3f(r, g, b), strength) {
}

VOLCANO_WORLD_END
