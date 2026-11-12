//
//
#include <Volcano/World/Common.h>
#include <Volcano/World/Context.h>
#include <Volcano/World/Scene.h>

VOLCANO_WORLD_BEGIN

Context& Scene::defaultContext() noexcept {
    static NullContext nullContext;
    return nullContext;
}

Scene::Scene() : Scene(defaultContext()) {}

Scene::Scene(Context& context) : context_(context) {}

Scene::~Scene() {
    clear();
}

void Scene::update(Clock::duration elapsed) {
    onUpdate(elapsed);
}

VOLCANO_WORLD_END
