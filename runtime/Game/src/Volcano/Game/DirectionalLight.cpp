//
//
#include <Volcano/Game/DirectionalLight.h>

VOLCANO_GAME_BEGIN

DirectionalLight::DirectionalLight(QObject* parent)
    : Light(parent) {
}

DirectionalLight::DirectionalLight(Context& context, QObject* parent)
    : Light(context, parent) {
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

VOLCANO_GAME_END
