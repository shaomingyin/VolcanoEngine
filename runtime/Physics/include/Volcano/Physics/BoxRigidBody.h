//
//
#ifndef VOLCANO_PHYSICS_BOXRIGIDBODY_H
#define VOLCANO_PHYSICS_BOXRIGIDBODY_H

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

class BoxRigidBody: public RigidBody {
public:
	BoxRigidBody(float mass, const Eigen::Vector3f& h);
	~BoxRigidBody() override = default;

public:
	float mass() const {
		return getMass();
	}

	float invMass() const {
		return getInvMass();
	}

	btBoxShape& shape() {
		return shape_;
	}

	const btBoxShape& shape() const {
		return shape_;
	}

private:
	Eigen::Affine3f offset_;
	btBoxShape shape_;
	btDefaultMotionState motion_state_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_BOXRIGIDBODY_H
