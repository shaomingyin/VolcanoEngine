//
//
#include <Volcano/Game/AmbientLight.h>

VOLCANO_GAME_BEGIN

AmbientLight::AmbientLight(QObject* parent)
    : Object(parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

AmbientLight::AmbientLight(Context& context, QObject* parent)
    : Object(context, parent)
    , color_(1.0f, 1.0f, 1.0f)
    , strength_(0.5f) {
}

QDataStream& operator<<(QDataStream& s, const AmbientLight& v) {
    s << static_cast<const Object&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, AmbientLight& v) {
    s >> static_cast<Object&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
