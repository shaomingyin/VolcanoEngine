//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <string>

#include <Volcano/World/Common.h>
#include <Volcano/World/Basic.h>

VOLCANO_WORLD_BEGIN

class Entity {
public:
	Entity(entt::registry& registry, std::string name = std::string());
	Entity(const Entity& other) = delete;
	Entity(Entity&& other);
	virtual ~Entity();

public:
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&);

	bool isEnabled() const {
		return basic().isEnabled();
	}

	void enable() {
		basic().enable();
	}

	void disable() {
		basic().disable();
	}

	bool isVisible() const {
		return basic().isVisible();
	}

	void show() {
		basic().show();
	}

	void hide() {
		basic().hide();
	}

	const std::string& name() const {
		return basic().name();
	}

	void rename(std::string v) {
		basic().rename(std::move(v));
	}

	Eigen::Affine3f transform() const {
		return basic().transform();
	}

	void setTransform(Eigen::Affine3f v) {
		basic().setTransform(v);
	}

	template <typename Component>
	Component& component() {
		VOLCANO_ASSERT(registry_ != nullptr);
		return registry_->get_or_emplace<Component>();
	}

	template <typename Component>
	const Component& component() const {
		VOLCANO_ASSERT(registry_ != nullptr);
		return registry_->get<Component>();
	}

	template <typename Component, typename... Args>
	Component& emplaceComponent(Args... args) {
		VOLCANO_ASSERT(registry_ != nullptr);
		return registry_->emplace<Component>(id_, std::forward<Args>(args)...);
	}

	template <typename Component>
	void removeComponent() {
		VOLCANO_ASSERT(registry_ != nullptr);
		return registry_->remove<Component>(id_);
	}

private:
	Basic& basic() {
		VOLCANO_ASSERT(registry_ != nullptr);
		VOLCANO_ASSERT(id_ != entt::null);
		return registry_->get<Basic>(id_);
	}

	const Basic& basic() const {
		VOLCANO_ASSERT(registry_ != nullptr);
		VOLCANO_ASSERT(id_ != entt::null);
		return registry_->get<Basic>(id_);
	}

private:
	entt::registry* registry_;
	entt::entity id_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
