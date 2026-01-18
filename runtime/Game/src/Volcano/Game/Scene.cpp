//
//
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::update(Clock::duration elapsed, Scheduler& scheduler) {
	auto updater = [&scheduler, elapsed](std::unique_ptr<Entity>& entity) {
		entity->update(elapsed, scheduler);
	};
	async::parallel_for(scheduler, async::make_range(entities_.begin(), entities_.end()), updater);
}

Graphics::VisibleSet Scene::buildVisibleSet(Scheduler& scheduler) const {
    auto map = [&scheduler](const std::unique_ptr<Entity>& entity) {
        return entity->buildVisibleSet(scheduler);
    };
    auto reduce = [&scheduler](Graphics::VisibleSet v1, Graphics::VisibleSet v2) {
        return std::move(v1) + std::move(v2);
    };
    return async::parallel_map_reduce(scheduler, async::make_range(entities_.begin(), entities_.end()), Graphics::VisibleSet(), map, reduce);
}

VOLCANO_GAME_END
