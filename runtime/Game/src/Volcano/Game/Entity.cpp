//
//
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::update(Clock::duration elapsed, Scheduler& scheduler) {
	auto updater = [&scheduler, elapsed](std::unique_ptr<Component>& component) {
		component->update(elapsed, scheduler);
	};
	async::parallel_for(scheduler, async::make_range(components_.begin(), components_.end()), updater);
}

Graphics::VisibleSet Entity::buildVisibleSet(Scheduler& scheduler) const {
    auto map = [&scheduler](const std::unique_ptr<Component>& component) {
        return component->buildVisibleSet(scheduler);
    };
    auto reduce = [&scheduler](Graphics::VisibleSet v1, Graphics::VisibleSet v2) {
        return std::move(v1) + std::move(v2);
    };
    return async::parallel_map_reduce(scheduler, async::make_range(components_.begin(), components_.end()), Graphics::VisibleSet(), map, reduce);
}

VOLCANO_GAME_END
