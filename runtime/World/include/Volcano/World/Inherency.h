//
//
#ifndef VOLCANO_WORLD_INHERENCY_H
#define VOLCANO_WORLD_INHERENCY_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Inherency {
public:
	Eigen::Affine3f& transform() {
		return transform_;
	}

	const Eigen::Affine3f& transform() const {
		return transform_;
	}

private:
	Eigen::Affine3f transform_ = Eigen::Affine3f::Identity();
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_INHERENCY_H
