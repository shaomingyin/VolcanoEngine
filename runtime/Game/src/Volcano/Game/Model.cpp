//
//
#include <Volcano/Game/Model.h>

VOLCANO_GAME_BEGIN

Model::Model(QObject* parent)
    : Loadable(parent) {
}

QDataStream& operator<<(QDataStream& s, const Model& v) {
    s << static_cast<const Loadable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Model& v) {
    s >> static_cast<Loadable&>(v);
    // TODO
    return s;
}

VOLCANO_GAME_END
