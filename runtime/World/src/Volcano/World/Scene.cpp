//
//
#include <Volcano/World/Common.h>
#include <Volcano/World/Context.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Scene::Scene() : Scene(defaultContext()) {}

Scene::Scene(Context& context) : context_(context) {}

Scene::~Scene() {
    clear();
}

void Scene::update(Clock::duration elapsed) {
    onUpdate(elapsed);
}

Context& Scene::defaultContext() noexcept {
    static NullContext nullContext;
    return nullContext;
}

VOLCANO_WORLD_END
