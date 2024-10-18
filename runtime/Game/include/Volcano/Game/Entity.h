//
//
#ifndef VOLCANO_GAME_ENTITY_H
#define VOLCANO_GAME_ENTITY_H

#include <string>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Basic.h>

VOLCANO_GAME_BEGIN

class Entity {
public:
	Entity(entt::registry& registry, int flags = 0);
	Entity(entt::registry& registry, std::string name, int flags = 0);
	Entity(const Entity& other) = delete;
	Entity(Entity&& other);
	virtual ~Entity();

public:
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&);

	template <typename Component, typename... Args>
	Component& emplaceComponent(Args... args) {
		return registry_->emplace<Component>(id_, std::forward<Args>(args)...);
	}

	template <typename Component>
	void removeComponent() {
		return registry_->remove<Component>(id_);
	}

private:
	entt::registry* registry_;
	entt::entity id_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_H
