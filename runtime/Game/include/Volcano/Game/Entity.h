//
//
#ifndef VOLCANO_GAME_ENTITY_H
#define VOLCANO_GAME_ENTITY_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Actor.h>

VOLCANO_GAME_BEGIN

class Entity: public Actor {
public:
	Entity(Context& context);
	~Entity() override;

public:
	entt::registry& registry() {
		return context().registry();
	}

	const entt::registry& registry() const {
		return context().registry();
	}

	entt::entity id() const {
		return id_;
	}

	template <typename Component>
	Component& component() {
		return registry().get<Component>(id_);
	}

	template <typename Component, typename... Args>
	Component& emplaceComponent(Args... args) {
		registry().emplace<Component>(id_, std::forward<Args>(args)...);
	}

	template <typename Component>
	void removeComponent() {
		registry().remove<Component>(id_);
	}

private:
	entt::entity id_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_H
