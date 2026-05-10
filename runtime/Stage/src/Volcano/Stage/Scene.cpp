//
//
#include <Volcano/STAGE/Inherent.h>
#include <Volcano/STAGE/Scene.h>

VOLCANO_STAGE_BEGIN

Scene::Scene() {
    on_construct<entt::entity>().connect<&Scene::onEntityAdded>(this);
}

Scene::~Scene() {
}

void Scene::onEntityAdded(entt::registry& registry, entt::entity entity) {
	emplace<Inherent>(entity);
}

VOLCANO_STAGE_END
