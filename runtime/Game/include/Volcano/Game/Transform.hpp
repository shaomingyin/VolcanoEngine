//
//
#ifndef VOLCANO_SIMULATION_TRANSFORM_HPP
#define VOLCANO_SIMULATION_TRANSFORM_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Simulation/Common.hpp>

VOLCANO_SIMULATION_BEGIN

class Transform: public Eigen::Affine3f {
public:
    Transform() = default;
};

VOLCANO_SIMULATION_END

#endif // VOLCANO_SIMULATION_TRANSFORM_HPP
