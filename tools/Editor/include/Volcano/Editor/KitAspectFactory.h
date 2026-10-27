//
//
#ifndef VOLCANO_EDITOR_KITASPECTFACTORY_H
#define VOLCANO_EDITOR_KITASPECTFACTORY_H

#include <projectexplorer/kitaspect.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class KitAspectFactory: public ProjectExplorer::KitAspectFactory {
public:
    KitAspectFactory(Context& context);

public:
    ProjectExplorer::Tasks validate(const ProjectExplorer::Kit* kit) const override;
    ItemList toUserOutput(const ProjectExplorer::Kit* kit) const override;
    ProjectExplorer::KitAspect *createKitAspect(ProjectExplorer::Kit* kit) const override;

private:
    Context& context_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_KITASPECTFACTORY_H
