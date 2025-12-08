//
//
#include <Volcano/Game/SpotLight.h>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
