//
//
#ifndef VOLCANO_GMAE_DIRECTIONALLIGHT_HPP
#define VOLCANO_GMAE_DIRECTIONALLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class DirectionalLight : public Light {
public:
	DirectionalLight();

public:
	const Eigen::Vector3f& direction() const noexcept {
		return direction_;
	}

private:
	Transform transform_;
	Eigen::Vector3f direction_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GMAE_DIRECTIONALLIGHT_HPP
