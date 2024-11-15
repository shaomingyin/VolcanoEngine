//
//
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene()
    : global_(*this, create()) {
}

Scene::~Scene() {
    global_.destroy();
}

VOLCANO_WORLD_END
