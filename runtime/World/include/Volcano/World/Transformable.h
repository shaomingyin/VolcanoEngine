//
//
#ifndef VOLCANO_WORLD_TRANSFORMABLE_H
#define VOLCANO_WORLD_TRANSFORMABLE_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Transformable {
public:
	Eigen::Affine3f& localTransform() {
		return local_transform_;
	}

	const Eigen::Affine3f& localTransform() const {
		return local_transform_;
	}

	Eigen::Affine3f toWorldTransform() const {
		if (world_transform_ != nullptr) {
			return (*world_transform_) * local_transform_;
		}
		return local_transform_;
	}

	void attachWorldTransform(Eigen::Affine3f* p);

private:
	Eigen::Affine3f* world_transform_ = nullptr;
	Eigen::Affine3f local_transform_ = Eigen::Affine3f::Identity();
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRANSFORMABLE_H
