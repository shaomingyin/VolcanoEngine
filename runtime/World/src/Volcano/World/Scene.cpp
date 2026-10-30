//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() = default;

Scene::~Scene() {
    clear();
}

void Scene::update(Clock::duration elapsed) {
    onUpdate(elapsed);
}

VOLCANO_WORLD_END
