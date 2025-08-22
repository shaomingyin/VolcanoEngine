//
//
#include <Volcano/Editor/KitAspect.h>
#include <Volcano/Editor/KitAspectFactory.h>

VOLCANO_EDITOR_BEGIN

KitAspectFactory::KitAspectFactory() {
    setDisplayName("VolcanoEngine");
    setId("volcanoeninge");
}

ProjectExplorer::Tasks KitAspectFactory::validate(const ProjectExplorer::Kit* kit) const {
    return {};
}

KitAspectFactory::ItemList KitAspectFactory::toUserOutput(const ProjectExplorer::Kit* kit) const {
    return {};
}

ProjectExplorer::KitAspect* KitAspectFactory::createKitAspect(ProjectExplorer::Kit* kit) const {
    return new KitAspect(kit, this);
}

VOLCANO_EDITOR_END
