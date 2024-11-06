//
//
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_trans) const {
	if (world_ != nullptr) {
		world_trans.setFromOpenGLMatrix(((*world_) * offset_).data());
	} else {
		world_trans.setFromOpenGLMatrix(offset_.data());
	}
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_trans) {
	if (world_ != nullptr) {
		Eigen::Affine3f world;
		world_trans.getOpenGLMatrix(world.data());
		*world_ = world * offset_.inverse();
	} else {
		world_trans.getOpenGLMatrix(offset_.data());
	}
}

RigidBody::RigidBody(float mass, btCollisionShape* shape)
	: btRigidBody(mass, &motion_state_, shape) {
}

void RigidBody::resetTransform() {
	if (motion_state_.world_ != nullptr) {
		auto offset = motion_state_.offset_;
		motion_state_.offset_ = (*motion_state_.world_) * offset;
		motion_state_.world_ = nullptr;
	}
}

void RigidBody::setTransform(Eigen::Affine3f& world) {
	resetTransform();
	motion_state_.world_ = &world;
	auto offset = motion_state_.offset_;
	motion_state_.offset_ = world.inverse() * offset;
}

VOLCANO_WORLD_END
