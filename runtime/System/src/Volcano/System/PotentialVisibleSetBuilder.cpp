//
//
#include <Volcano/System/PotentialVisibleSetBuilder.h>

VOLCANO_SYSTEM_BEGIN

PotentialVisibleSetBuilder::PotentialVisibleSetBuilder(const World::Scene& scene, QObject* parent)
    : QObject(parent)
    , scene_(scene) {
}

void PotentialVisibleSetBuilder::build(Graphics::View& view) const {
}

VOLCANO_SYSTEM_END
