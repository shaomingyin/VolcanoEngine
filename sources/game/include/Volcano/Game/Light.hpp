//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

struct Light: public Entity {
    sigslot::signal<float> setStrength;
    sigslot::signal<const Eigen::Vector3f &> setColor;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
