//
//
#include <Volcano/SpotLight.hpp>

VOLCANO_BEGIN

SpotLight::SpotLight(QObject* parent)
    : DirectionalLight(parent) {
}

SpotLight::SpotLight(Context& context, QObject* parent)
    : DirectionalLight(context, parent) {
}

VOLCANO_END
