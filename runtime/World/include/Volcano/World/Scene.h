//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class Scene {
public:
	Scene();
	~Scene();

public:
	void update(Clock::duration elapsed);

private:

private:
	JPH::PhysicsSystem physics_system_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
