//
//
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

const Transform& Transform::identity() {
    static const Transform Identity = Affine3::identity();
    return Identity;
}

QDataStream& operator<<(QDataStream& s, const Transform& v) {
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Transform& v) {
    // TODO
    return s;
}

VOLCANO_WORLD_END
