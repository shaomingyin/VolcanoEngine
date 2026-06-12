//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() {
	on_construct<entt::entity>().connect<&Scene::onEntityAdded>(this);
	on_destroy<entt::entity>().connect<&Scene::onEntityRemoved>(this);
}

Scene::~Scene() {
	clear();
}

const std::string& Scene::name() const noexcept {
	static const std::string ret(std::format("Scene{}", intptr_t(this)));
	return ret;
}

const std::string& Scene::description() const noexcept {
	static const std::string ret("<No description>");
	return ret;
}

const VersionNumber& Scene::version() const noexcept {
	static VersionNumber ret(0, 0, 0);
	return ret;
}

entt::entity Scene::mainCamera() const noexcept {
	return entt::null;
}

void Scene::load(nlohmann::json&& json) {
}

void Scene::update(Clock::duration elapsed) noexcept {
}

void Scene::onEntityAdded(entt::entity ent) noexcept {
}

void Scene::onEntityRemoved(entt::entity ent) noexcept {
}

VOLCANO_WORLD_END
