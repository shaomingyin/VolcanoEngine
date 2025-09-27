//
//
#include <Volcano/Editor/KitAspect.h>
#include <Volcano/Editor/KitAspectFactory.h>

VOLCANO_EDITOR_BEGIN

KitAspectFactory::KitAspectFactory(Context& context)
    : context_(context) {
    setDisplayName("Volcano Engine Version");
    setId("Volcano.Editor.KeyAspect");
}

ProjectExplorer::Tasks KitAspectFactory::validate(const ProjectExplorer::Kit* kit) const {
    return {};
}

KitAspectFactory::ItemList KitAspectFactory::toUserOutput(const ProjectExplorer::Kit* kit) const {
    return {};
}

ProjectExplorer::KitAspect* KitAspectFactory::createKitAspect(ProjectExplorer::Kit* kit) const {
    return new KitAspect(context_, kit, this);
}

VOLCANO_EDITOR_END
