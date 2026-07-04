//
//
#include <nlohmann/json.hpp>

#include <Volcano/Math.h>
#include <Volcano/FileSystem.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene(const nlohmann::json& metadata) {
	on_construct<entt::entity>().connect<&Scene::handleEntityAdded>(this);
	on_destroy<entt::entity>().connect<&Scene::handleEntityRemoved>(this);
}

Scene::~Scene() {
	clear();
	on_construct<entt::entity>().disconnect<&Scene::handleEntityAdded>(this);
	on_destroy<entt::entity>().disconnect<&Scene::handleEntityRemoved>(this);
}

void Scene::load(const nlohmann::json& metadata) {
	name_ = metadata["name"].get<std::string>();
	description_ = metadata["description"].get<std::string>();
	version_ = metadata["version"].get<VersionNumber>();

	auto& entities = metadata.at("entities");
	if (!entities.is_array()) {
		throw std::runtime_error("Scene metadata 'entities' field is not an array.");
	}

	for (const auto& entity : entities) {
		auto ent = create();
		patch<Inherent>(ent, [&entity](Inherent& v) {
			v = entity.get<Inherent>();
			});
		auto transform = metadata.find("transform");
		if (transform != metadata.end()) {
			emplace<Transform>(ent, transform->get<Transform>());
		}
	}
}

void Scene::handleEntityAdded(entt::entity ent) noexcept {
	emplace<Inherent>(ent);
}

void Scene::handleEntityRemoved(entt::entity ent) noexcept {
}

VOLCANO_WORLD_END
