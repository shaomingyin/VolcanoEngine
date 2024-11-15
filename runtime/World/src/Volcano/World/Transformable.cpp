//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

void Transformable::attachWorldTransform(Eigen::Affine3f* p) {
	if (world_transform_ != nullptr) {
		auto offset = local_transform_;
		local_transform_ = (*p) * offset;
		world_transform_ = nullptr;
	}
	if (p != nullptr) {
		auto offset = local_transform_;
		local_transform_ = p->inverse() * offset;
		world_transform_ = p;
	}
}

VOLCANO_WORLD_END
