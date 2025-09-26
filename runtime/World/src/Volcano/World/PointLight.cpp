//
//
#include <Volcano/World/PointLight.h>

VOLCANO_WORLD_BEGIN

PointLight::PointLight(QObject* parent)
    : Light(parent) {
}

QDataStream& operator<<(QDataStream& s, const PointLight& v) {
    s << static_cast<const Light&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, PointLight& v) {
    s >> static_cast<Light&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
