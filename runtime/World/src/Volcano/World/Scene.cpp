//
//
#include <Volcano/Math.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() {
}

Scene::~Scene() {
	clear();
}

void Scene::update(Clock::duration elapsed) noexcept {
}

VOLCANO_WORLD_END
