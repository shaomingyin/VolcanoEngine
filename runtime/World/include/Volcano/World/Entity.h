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
		VOLCANO_ASSERT(basic_ != nullptr);
		return basic_->isEnabled();
	}

	void enable() {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->enable();
	}

	void disable() {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->disable();
	}

	bool isVisible() const {
		VOLCANO_ASSERT(basic_ != nullptr);
		return basic_->isVisible();
	}

	void show() {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->show();
	}

	void hide() {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->hide();
	}

	const std::string& name() const {
		VOLCANO_ASSERT(basic_ != nullptr);
		return basic_->name();
	}

	void rename(std::string v) {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->rename(std::move(v));
	}

	Eigen::Affine3f transform() const {
		VOLCANO_ASSERT(basic_ != nullptr);
		return basic_->transform();
	}

	void setTransform(Eigen::Affine3f v) {
		VOLCANO_ASSERT(basic_ != nullptr);
		basic_->setTransform(v);
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

	virtual void loadFromJson(const nlohmann::json& j);
	virtual void saveToJson(nlohmann::json& j);

private:
	entt::registry* registry_;
	entt::entity id_;
	Basic* basic_; // FIXME: Is this pointer stable?
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
