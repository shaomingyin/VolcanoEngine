//
//
#ifndef VOLCANO_WORLD_BOXRIGIDBODY_H
#define VOLCANO_WORLD_BOXRIGIDBODY_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

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

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXRIGIDBODY_H
