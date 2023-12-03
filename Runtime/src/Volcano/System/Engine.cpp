//
//
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

Engine::Engine(QObject* parent)
    : Context(parent) {
}

bool Engine::init() {
    return true;
}

ResourceManager& Engine::resourceManager() {
    return resourceManager_;
}

VOLCANO_SYSTEM_END
