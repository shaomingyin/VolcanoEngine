//
//
#include <Volcano/Game/Entity.h>

VOLCANO_GAME_BEGIN

Entity::Entity(entt::registry& registry, int flags)
    : Entity(registry, std::string(), flags) {
}

Entity::Entity(entt::registry& registry, std::string name, int flags)
    : registry_(&registry)
    , id_(registry.create()) {
    auto& basic = registry.emplace<Basic>(id_, std::move(name), flags);
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

VOLCANO_GAME_END
