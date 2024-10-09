//
//
#ifndef VOLCANO_PHYSICS_BULLET_WORLD_H
#define VOLCANO_PHYSICS_BULLET_WORLD_H

#include <Volcano/Physics/World.h>
#include <Volcano/Physics/Bullet/Common.h>

VOLCANO_PHYSICS_BULLET_BEGIN

class World: public Physics::World {
public:
    World();
    ~World() override;

public:
    //bool init() override;
    void setGravity(Eigen::Vector3f v) override;
    void step(std::chrono::steady_clock::duration elapsed) override;

private:
    btDynamicsWorld* world_;
};

VOLCANO_PHYSICS_BULLET_END

#endif // VOLCANO_PHYSICS_BULLET_WORLD_H
