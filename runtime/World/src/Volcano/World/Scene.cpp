//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
	: physics_runtime_((Physics::init(), &Physics::shutdown))
    , physics_(*this) {
}

Scene::~Scene() {
	clear();
}

void Scene::update(Clock::duration elapsed) noexcept {
	onUpdate(elapsed);
    if (elapsed > Clock::duration::zero()) {
        physics_.update(elapsed);
    }
}

VOLCANO_WORLD_END
