//
//
#include <QLabel>

#include <qtsupport/qtkitaspect.h>

#include <Volcano/Editor/KitAspect.h>

VOLCANO_EDITOR_BEGIN

KitAspect::KitAspect(ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory)
    : ProjectExplorer::KitAspect(kit, factory) {
    setDisplayName("VolcanoEngine");
    setConfigWidgetCreator([this] {
        return new QLabel("test");
    });
}

VOLCANO_EDITOR_END
