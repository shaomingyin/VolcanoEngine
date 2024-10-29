//
//
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

DirectionalLight::DirectionalLight()
    : direction_(0.0f, -1.0f, 0.0f) {
}

DirectionalLight::~DirectionalLight() {
}

VOLCANO_WORLD_END
