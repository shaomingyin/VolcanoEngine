//
//
#include <Volcano/Game/Material.h>

VOLCANO_GAME_BEGIN

Material::Material(QObject* parent)
    : Component(parent) {
}

QDataStream& operator<<(QDataStream& s, const Material& v) {
    s << static_cast<const Component&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Material& v) {
    s >> static_cast<Component&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
