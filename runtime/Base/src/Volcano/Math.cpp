//
//
#include <Volcano/Math.h>

VOLCANO_BEGIN

const Affine3& Affine3::identity() {
    static const Affine3 Identity = Eigen::Affine3f::Identity();
    return Identity;
}

VOLCANO_END
