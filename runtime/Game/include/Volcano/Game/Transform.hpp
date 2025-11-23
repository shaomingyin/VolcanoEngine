//
//
#ifndef VOLCANO_GAME_TRANSFORM_HPP
#define VOLCANO_GAME_TRANSFORM_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Transform: public Eigen::Affine3f {
public:
    Transform() = default;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRANSFORM_HPP
