//
//
#include <qtsupport/qtkitaspect.h>
#include <Volcano/Editor/KitAspect.h>

VOLCANO_EDITOR_BEGIN

KitAspect::KitAspect(ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory)
    : ProjectExplorer::KitAspect(kit, factory) {
    //setConfigWidgetCreator([this] {
        // new config widget.
    //});
}

VOLCANO_EDITOR_END
