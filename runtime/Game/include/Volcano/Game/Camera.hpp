//
//
#ifndef VOLCANO_GAME_CAMERA_HPP
#define VOLCANO_GAME_CAMERA_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Camera : public Component {
public:
	Camera();

public:

private:
	Eigen::Vector3f position_;
	Eigen::Vector3f direction_;
	Eigen::Vector3f up_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAMERA_HPP
