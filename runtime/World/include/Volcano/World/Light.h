//
//
#ifndef VOLCANO_WORLD_LIGHT_H
#define VOLCANO_WORLD_LIGHT_H

#include <variant>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

struct Light {
	struct Directional {
		Eigen::Vector3f direction;
	};

	struct Point {
		float radius;
	};

	struct Spot {
		float radius;
		// TODO
	};

	std::variant<Directional, Point, Spot> def;
	Eigen::Vector4f color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float strength_ = { 1.0f };
	bool on;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_LIGHT_H
