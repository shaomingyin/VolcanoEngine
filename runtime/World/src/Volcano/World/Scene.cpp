//
//
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
	: physics_(*this) {
	on_construct<entt::entity>().connect<&Scene::onEntityAdded>(this);
	on_destroy<entt::entity>().connect<&Scene::onEntityRemoved>(this);
}

void Scene::update(Clock::duration elapsed) noexcept {
	physics_.update(elapsed);
}

void Scene::onEntityAdded(entt::entity ent) noexcept {
	//emplace<Inherent>(ent);
}

void Scene::onEntityRemoved(entt::entity ent) noexcept {
}

VOLCANO_WORLD_END
