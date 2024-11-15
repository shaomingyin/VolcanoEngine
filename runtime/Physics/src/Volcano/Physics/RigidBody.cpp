//
//
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

void RigidBody::MotionState::getWorldTransform(btTransform& world_trans) const {
	if (world != nullptr) {
		world_trans.setFromOpenGLMatrix(((*world) * offset).data());
	} else {
		world_trans.setFromOpenGLMatrix(offset.data());
	}
}

void RigidBody::MotionState::setWorldTransform(const btTransform& world_trans) {
	if (world != nullptr) {
		Eigen::Affine3f w;
		world_trans.getOpenGLMatrix(w.data());
		*world = w * offset.inverse();
	} else {
		world_trans.getOpenGLMatrix(offset.data());
	}
}

RigidBody::RigidBody(float mass, btCollisionShape* shape)
	: btRigidBody(mass, nullptr, nullptr) {
	setCollisionShape(shape);
	setMotionState(&motion_state_);
}

void RigidBody::resetWorldTransform() {
	if (motion_state_.world != nullptr) {
		auto offset = motion_state_.offset;
		motion_state_.offset = (*motion_state_.world) * offset;
		motion_state_.world = nullptr;
	}
}

void RigidBody::attachWorldTransform(Eigen::Affine3f& world) {
	resetWorldTransform();
	motion_state_.world = &world;
	auto offset = motion_state_.offset;
	motion_state_.offset = world.inverse() * offset;
}

VOLCANO_PHYSICS_END
