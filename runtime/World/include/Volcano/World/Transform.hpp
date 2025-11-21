//
//
#ifndef VOLCANO_WORLD_TRANSFORM_HPP
#define VOLCANO_WORLD_TRANSFORM_HPP

#include <Volcano/Math.hpp>
#include <Volcano/World/Common.hpp>

VOLCANO_WORLD_BEGIN

class Transform: public Eigen::Affine3f {
public:
    Transform() = default;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORM_HPP
