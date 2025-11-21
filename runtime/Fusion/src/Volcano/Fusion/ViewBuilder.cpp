//
//
#include <Volcano/Fusion/Renderable.hpp>
#include <Volcano/Fusion/ViewBuilder.hpp>

VOLCANO_FUSION_BEGIN

ViewBuilder::ViewBuilder(const World::Scene& scene)
    : scene_(scene) {
}

void ViewBuilder::build(Graphics::View& view) {
}

VOLCANO_FUSION_END
