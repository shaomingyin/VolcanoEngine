//
//
#ifndef VOLCANO_EDITOR_KITASPECT_H
#define VOLCANO_EDITOR_KITASPECT_H

#include <QString>

#include <projectexplorer/kitaspect.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class KitAspect: public ProjectExplorer::KitAspect {
    Q_OBJECT

public:
    KitAspect(Context& context, ProjectExplorer::Kit* kit, const ProjectExplorer::KitAspectFactory* factory);

public:

private:
    Context& context_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITASPECT_H
