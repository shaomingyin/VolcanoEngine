//
//
#ifndef VOLCANO_GAME_CAMERA_HPP
#define VOLCANO_GAME_CAMERA_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Camera: public Object {
public:
    sigslot::signal<const Eigen::Vector3f &> setPosition;
    sigslot::signal<const Eigen::Vector3f &> setDirection;
    sigslot::signal<const Eigen::Vector3f &> setUp;
    sigslot::signal<const Eigen::AlignedBox2f &> setRect;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAMERA_HPP
