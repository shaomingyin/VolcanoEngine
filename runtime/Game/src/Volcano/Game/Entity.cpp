//
//
#include <Volcano/Game/Entity.h>

VOLCANO_GAME_BEGIN

Entity::Entity(entt::registry& registry, std::string name)
    : registry_(&registry)
    , id_(registry.create())
    , basic_(&(registry.emplace<Basic>(id_, std::move(name)))) {
}

Entity::Entity(const Entity& other)
    : registry_(other.registry_)
    , id_(other.id_)
    , basic_(other.basic_) {
}

Entity::~Entity() {
    VOLCANO_ASSERT(registry_ != nullptr);
    registry_->destroy(id_);
}

Entity& Entity::operator=(const Entity& other) {
    if (this != &other) {
        VOLCANO_ASSERT(other.registry_ != nullptr);
        registry_ = other.registry_;
        VOLCANO_ASSERT(other.basic_ != nullptr);
        basic_ = other.basic_;
        id_ = other.id_;
    }
    return (*this);
}

VOLCANO_GAME_END
