//
//
#include <Volcano/World/DirectionalLight.h>

VOLCANO_WORLD_BEGIN

DirectionalLight::DirectionalLight(QObject* parent)
    : Light(parent) {
}

QDataStream& operator<<(QDataStream& s, const DirectionalLight& v) {
    s << static_cast<const Light&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, DirectionalLight& v) {
    s >> static_cast<Light&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
