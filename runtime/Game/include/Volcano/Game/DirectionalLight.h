//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_H
#define VOLCANO_GAME_DIRECTIONALLIGHT_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

class DirectionalLight: public Light {
public:
	DirectionalLight(Context& context);
	~DirectionalLight() override;

public:
	Eigen::Vector3f direction() const {
		return direction_;
	}

	void setDirection(Eigen::Vector3f v) {
		direction_ = v;
	}

private:
	Eigen::Vector3f direction_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_H
