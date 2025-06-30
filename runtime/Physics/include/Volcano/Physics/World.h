//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class World {
	RTTR_ENABLE()

public:
	World();
	virtual ~World() = default;

public:
	const Eigen::Vector3f& gravity() const noexcept {
		return gravity_;
	}

	virtual void setGravity(const Eigen::Vector3f& v);
	virtual bool isEnabled() const noexcept = 0;
	virtual void enable() noexcept = 0;
	virtual void disable() noexcept = 0;

private:
	Eigen::Vector3f gravity_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
