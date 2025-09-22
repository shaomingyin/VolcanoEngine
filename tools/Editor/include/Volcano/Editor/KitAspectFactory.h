//
//
#ifndef VOLCANO_EDITOR_KITASPECTFACTORY_H
#define VOLCANO_EDITOR_KITASPECTFACTORY_H

#include <projectexplorer/kitaspect.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class KitAspectFactory: public ProjectExplorer::KitAspectFactory {
public:
    KitAspectFactory();

public:
    ProjectExplorer::Tasks validate(const ProjectExplorer::Kit* kit) const override;
    ItemList toUserOutput(const ProjectExplorer::Kit* kit) const override;
    ProjectExplorer::KitAspect *createKitAspect(ProjectExplorer::Kit* kit) const override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITASPECTFACTORY_H
