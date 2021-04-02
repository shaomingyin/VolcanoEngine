//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

struct DirectionalLight: public Light {
    sigslot::signal<const Eigen::Vector3f &> setDirection;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_HPP
