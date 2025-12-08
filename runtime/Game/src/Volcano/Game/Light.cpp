//
//
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

Light::Light(QObject* parent)
    : Transformable(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

QDataStream& operator<<(QDataStream& s, const Light& v) {
    s << static_cast<const Transformable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Light& v) {
    s >> static_cast<Transformable&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
