//
//
#ifndef VOLCANO_GAME_DIRECTIONALLGIHT_HPP
#define VOLCANO_GAME_DIRECTIONALLGIHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class DirectionalLight: public Light{
public:
    DirectionalLight();

public:

private:
    Eigen::Vector3 direction_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLGIHT_HPP
