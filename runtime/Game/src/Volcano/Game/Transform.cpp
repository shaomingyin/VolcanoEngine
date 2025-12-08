//
//
#include <Volcano/Game/Transform.h>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
