//
//
#ifndef VOLCANO_PHYSICS_RIGIDBODY_H
#define VOLCANO_PHYSICS_RIGIDBODY_H

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class RigidBody: public btRigidBody {
public:
	RigidBody(float mass = 0.0f, btCollisionShape* shape = nullptr);
	virtual ~RigidBody() = default;

public:
	Eigen::Affine3f& offset() {
		return motion_state_.offset;
	}

	const Eigen::Affine3f& offset() const {
		return motion_state_.offset;
	}

	void resetWorldTransform();
	void attachWorldTransform(Eigen::Affine3f& world);

private:
	struct MotionState final: public btMotionState {
		Eigen::Affine3f* world = nullptr;
		Eigen::Affine3f offset = Eigen::Affine3f::Identity();
		void getWorldTransform(btTransform& world_trans) const override;
		void setWorldTransform(const btTransform& world_trans) override;
	};
	
	MotionState motion_state_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_RIGIDBODY_H
