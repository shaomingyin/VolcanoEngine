//
//
#include <Volcano/Game/Transform.h>

VOLCANO_GAME_BEGIN

const Transform& Transform::identity() {
    static const Transform Identity = Affine3::identity();
    return Identity;
}

VOLCANO_GAME_END
