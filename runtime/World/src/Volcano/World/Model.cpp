//
//
#include <Volcano/World/Model.h>

VOLCANO_WORLD_BEGIN

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

VOLCANO_WORLD_END
