//
//
#ifndef VOLCANO_WORLD_SCENE_H
#define VOLCANO_WORLD_SCENE_H

#include <memory>
#include <vector>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <entt/entt.hpp>

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class Scene: public entt::registry {
public:
	Scene();
	~Scene();

public:
	entt::handle global() const noexcept {
		return global_;
	}

	Light& ambientLight() noexcept {
		return ambient_light_;
	}

	const Light& ambientLight() const noexcept {
		return ambient_light_;
	}

	void update(Clock::duration elapsed);

private:
	void onTransformAdded(entt::entity ent);
	void onTransformRemoved(entt::entity ent);
	void onTransformUpdated(entt::entity ent);
	void onKineticsAdded(entt::entity ent);
	void onKineticsRemoved(entt::entity ent);
	void onKineticsUpdated(entt::entity ent);

private:
	entt::handle global_;
	Light ambient_light_;
	JPH::PhysicsSystem physics_system_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCENE_H
