//
//
#include <Volcano/World/Transform.h>

VOLCANO_WORLD_BEGIN

const Transform& Transform::identity() {
    static const Transform Identity = Affine3::identity();
    return Identity;
}

VOLCANO_WORLD_END
