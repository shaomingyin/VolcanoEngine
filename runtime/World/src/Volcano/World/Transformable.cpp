//
//
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

void Transformable::attachWorldTransform(Eigen::Affine3f* p) {
	if (world_transform_ != nullptr) {
		auto offset = local_transform_;
		local_transform_ = *world_transform_ * offset;
		world_transform_ = nullptr;
	}
	if (p != nullptr) {
		auto offset = local_transform_;
		local_transform_ = p->inverse() * offset;
		world_transform_ = p;
	}
}

void Transformable::getWorldTransform(btTransform& world_trans) const {
	if (world_transform_ != nullptr) {
		auto wt = (*world_transform_) * local_transform_;
		world_trans.setFromOpenGLMatrix(wt.data());
	} else {
		world_trans.setFromOpenGLMatrix(local_transform_.data());
	}
}

void Transformable::setWorldTransform(const btTransform& world_trans) {
	if (world_transform_ != nullptr) {
		Eigen::Affine3f tmp;
		world_trans.getOpenGLMatrix(tmp.data());
		local_transform_ = world_transform_->inverse() * tmp;
	} else {
		world_trans.getOpenGLMatrix(local_transform_.data());
	}
}

VOLCANO_WORLD_END
