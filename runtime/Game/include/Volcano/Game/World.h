//
//
#ifndef VOLCANO_GAME_WORLD_H
#define VOLCANO_GAME_WORLD_H

#include <memory>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Entity.h>
#include <Volcano/Game/Light.h>
#include <Volcano/Game/Camera.h>

VOLCANO_GAME_BEGIN

class World {
public:
	World();
	World(const World&) = delete;
	virtual ~World();

public:
	World& operator=(const World&) = delete;

	void update(Duration elapsed);

	[[nodiscard]] Entity createEntity(std::string name = std::string()) {
		return Entity(registry_, std::move(name));
	}

	template <typename Component>
	auto view() {
		return registry_.view<Component>();
	}

	template <typename... Components>
	auto view() {
		return registry_.view<Components...>();
	}

	Light& ambientLight() {
		return registry_.get<Light>(builtin_);
	}

	const Light& ambientLight() const {
		return registry_.get<Light>(builtin_);
	}

	Camera& camera() {
		return registry_.get_or_emplace<Camera>(builtin_);
	}

	const Camera& camera() const {
		return registry_.get<Camera>(builtin_);
	}

	bool isPhysicsEnabled() {
		return bool(bt_dynamics_world_);
	}

	void enablePhysics();
	void disablePhysics();

	Eigen::Vector3f gravity() const {
		return gravity_;
	}

	void setGravity(Eigen::Vector3f v);

protected:
	entt::registry& registry() {
		return registry_;
	}

	const entt::registry& registry() const {
		return registry_;
	}

private:
	void rigidBodyCreated(entt::registry& reg, entt::entity ent);
	void rigidBodyDestroyed(entt::registry& reg, entt::entity ent);

private:
	entt::registry registry_;
	entt::entity builtin_;
	Eigen::Vector3f gravity_;
	std::unique_ptr<btDynamicsWorld> bt_dynamics_world_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_H
