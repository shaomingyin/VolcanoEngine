//
//
#include <Volcano/World/SpotLight.h>

VOLCANO_WORLD_BEGIN

SpotLight::SpotLight(QObject* parent)
    : DirectionalLight(parent) {
}

QDataStream& operator<<(QDataStream& s, const SpotLight& v) {
    s << static_cast<const DirectionalLight&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, SpotLight& v) {
    s >> static_cast<DirectionalLight&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
