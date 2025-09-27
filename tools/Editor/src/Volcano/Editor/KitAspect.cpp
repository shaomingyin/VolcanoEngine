//
//
#include <QLabel>

#include <utils/layoutbuilder.h>
#include <qtsupport/qtkitaspect.h>

#include <Volcano/Editor/KitAspect.h>

VOLCANO_EDITOR_BEGIN

const Utils::Id KitAspect::Id("Volcano.Editor.KitAspect");

KitAspect::KitAspect(Context& context, ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory)
    : ProjectExplorer::KitAspect(kit, factory)
    , context_(context)
    , label1_("testLabel1")
    , label2_("testLabel2") {
    setId(Id);
    setDisplayName("Volcano Engine Version");
    setAutoApply(false);
}

void KitAspect::addToInnerLayout(Layouting::Layout &layout) {
    ProjectExplorer::KitAspect::addToInnerLayout(layout);
    addManageButtonToLayout(layout);
}

VOLCANO_EDITOR_END
