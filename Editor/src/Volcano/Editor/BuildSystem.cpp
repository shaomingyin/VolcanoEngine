//
//
#include <memory>

#include <Volcano/Editor/ProjectNode.hpp>
#include <Volcano/Editor/BuildSystem.hpp>

VOLCANO_EDITOR_BEGIN

BuildSystem::BuildSystem(ProjectExplorer::Target *target):
    ProjectExplorer::BuildSystem(target)
{
    qDebug() << __FUNCTION__;
    setRootProjectNode(std::make_unique<ProjectNode>(projectFilePath()));
}

BuildSystem::~BuildSystem(void)
{
}

QString BuildSystem::name(void) const
{
    qDebug() << __FUNCTION__;
    return "VolcanoBuildSystem";
}

void BuildSystem::triggerParsing(void)
{
    qDebug() << __FUNCTION__;
    qDebug() << project()->rootProjectNode();
}

VOLCANO_EDITOR_END
