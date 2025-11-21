//
//
#include <Volcano/Simulation/Transform.hpp>
#include <Volcano/Utils/Renderable.hpp>
#include <Volcano/Utils/ViewBuilder.hpp>

VOLCANO_UTILS_BEGIN

ViewBuilder::ViewBuilder(const Simulation::World& world)
    : world_(world) {
}

void ViewBuilder::build(Graphics::View& view) {
    for (auto& scene: world_.scenes()) {
        // for(auto [transform, renderable]: scene.view<Simulation::Transform, Renderable>().each()) {
        // }
    }
}

VOLCANO_UTILS_END
