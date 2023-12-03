//
//
#include <Volcano/Game/SpotLight.hpp>

VOLCANO_GAME_BEGIN

SpotLight::SpotLight(QObject* parent)
    : DirectionalLight(parent) {
}

SpotLight::SpotLight(Context& context, QObject* parent)
    : DirectionalLight(context, parent) {
}

VOLCANO_GAME_END
