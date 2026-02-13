//
//
#ifndef VOLCANO_WORLD_TRANSFORM_H
#define VOLCANO_WORLD_TRANSFORM_H

#include <Volcano/Math.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Transform: public Eigen::Affine3f {
public:
    Transform() = default;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_H
