//
//
#include <edyn/edyn.hpp>

#include <Volcano/Simulation/Scene.hpp>

VOLCANO_SIMULATION_BEGIN

Scene::Scene()
    : global_(create()) {
}

Scene::~Scene() {
}

VOLCANO_SIMULATION_END
