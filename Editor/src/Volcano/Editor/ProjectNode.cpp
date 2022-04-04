//
//
#include <Volcano/Editor/ProjectNode.hpp>

VOLCANO_EDITOR_BEGIN

ProjectNode::ProjectNode(const Utils::FilePath &projectFilePath):
    ProjectExplorer::ProjectNode(projectFilePath)
{
    setProductType(ProjectExplorer::ProductType::App);
    setDisplayName("Game");
}

VOLCANO_EDITOR_END
