//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

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

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
