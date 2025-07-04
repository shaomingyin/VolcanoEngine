//
//
#include <Volcano/World/Entity.h>

VOLCANO_WORLD_BEGIN

Entity::Entity(entt::registry& registry, std::string name)
    : registry_(&registry)
    , id_(registry.create()) {
    registry.emplace<Basic>(id_, std::move(name));
}

Entity::Entity(Entity&& other)
    : registry_(other.registry_)
    , id_(other.id_) {
    VOLCANO_ASSERT(registry_ != nullptr);
    other.registry_ = nullptr;
    other.id_ = entt::null;
}

Entity::~Entity() {
    VOLCANO_ASSERT(registry_ != nullptr);
    registry_->destroy(id_);
}

Entity& Entity::operator=(Entity&& other) {
    if (this != &other) {
        registry_ = other.registry_;
        other.registry_ = nullptr;
        id_ = other.id_;
        other.id_ = entt::null;
    }
    return (*this);
}

VOLCANO_WORLD_END
