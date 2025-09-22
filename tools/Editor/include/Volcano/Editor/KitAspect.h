//
//
#ifndef VOLCANO_EDITOR_KITASPECT_H
#define VOLCANO_EDITOR_KITASPECT_H

#include <projectexplorer/kitaspect.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class KitAspect: public ProjectExplorer::KitAspect {
    Q_OBJECT

public:
    KitAspect(ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITASPECT_H
