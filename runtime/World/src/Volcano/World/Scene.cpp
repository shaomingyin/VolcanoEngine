//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() {
	on_construct<entt::entity>().connect<&Scene::onEntityAdded>(this);
	on_destroy<entt::entity>().connect<&Scene::onEntityRemoved>(this);
}

entt::entity Scene::mainCamera() const noexcept {
	return entt::null;
}

void Scene::onEntityAdded(entt::entity ent) noexcept {
	emplace<Inherent>(ent);
}

void Scene::onEntityRemoved(entt::entity ent) noexcept {
}

VOLCANO_WORLD_END
