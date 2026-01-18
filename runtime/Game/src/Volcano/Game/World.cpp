//
//
#include <async++.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

World::World() {
}

World::~World() {
}

void World::update(Clock::duration elapsed, Scheduler& scheduler) {
    dynamic_.update(elapsed);
    // TODO
    auto updater = [&scheduler, elapsed](std::unique_ptr<Scene>& scene) {
        scene->update(elapsed, scheduler);
    };
    async::parallel_for(scheduler, async::make_range(scenes_.begin(), scenes_.end()), updater);
}

Graphics::VisibleSet World::buildVisibleSet(Scheduler& scheduler) const {
    auto map = [&scheduler](const std::unique_ptr<Scene>& scene) {
        return scene->buildVisibleSet(scheduler);
    };
    auto reduce = [&scheduler](Graphics::VisibleSet v1, Graphics::VisibleSet v2) {
        return std::move(v1) + std::move(v2);
    };
    return async::parallel_map_reduce(scheduler, async::make_range(scenes_.begin(), scenes_.end()), Graphics::VisibleSet(), map, reduce);
}

Graphics::View World::buildView(Scheduler& scheduler) const {
    Graphics::View view;
    view.merge(buildVisibleSet(scheduler));
    // main_camera
    // ambient_light
    // sun_light
    return view;
}

VOLCANO_GAME_END
