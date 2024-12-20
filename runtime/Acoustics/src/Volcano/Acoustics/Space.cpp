//
//
#include <Volcano/Acoustics/Space.h>

VOLCANO_ACOUSTICS_BEGIN

Space::Space(World::Scene& scene, QObject* parent)
    : World::Listener(scene, parent) {
}

void Space::update(Duration elapsed) {
}

void Space::render() {
}

void Space::onComponentAdded(World::Entity* entity, World::Component* component) {
}

void Space::onComponentRemoved(World::Entity* entity, World::Component* component) {
}

VOLCANO_ACOUSTICS_END
