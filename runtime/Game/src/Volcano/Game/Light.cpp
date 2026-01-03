//
//
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

Light::Light()
    : Light({ 1.0f, 1.0f, 1.0f, 1.0f }) {
}

Light::Light(const Eigen::Vector4f& color)
    : color_(color) {
}

VOLCANO_GAME_END
