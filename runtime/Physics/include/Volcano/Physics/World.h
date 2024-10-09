//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

class World {
public:
    World();
    virtual ~World();

public:
    const Eigen::Vector3f gravity() const {
        return gravity_;
    }

    virtual void setGravity(Eigen::Vector3f v) {
        gravity_ = v;
    }

    virtual void step(std::chrono::steady_clock::duration elapsed) = 0;

private:
    Eigen::Vector3f gravity_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
