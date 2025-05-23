//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
    : global_(create()) {
}

void Scene::loadFromJson(const nlohmann::json& j) {
}

void Scene::saveToJson(nlohmann::json& j) {
}

VOLCANO_WORLD_END
