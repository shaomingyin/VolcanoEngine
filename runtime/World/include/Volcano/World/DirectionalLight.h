//
//
#ifndef VOLCANO_WORLD_DIRECTIONALLIGHT_H
#define VOLCANO_WORLD_DIRECTIONALLIGHT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class DirectionalLight: public Light {
public:
	DirectionalLight();
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

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_DIRECTIONALLIGHT_H
