//
//
#include <Volcano/Editor/ProjectNode.hpp>

VOLCANO_EDITOR_BEGIN

ProjectNode::ProjectNode(const Utils::FilePath &path)
    : ProjectExplorer::ProjectNode(path) {
    setProductType(ProjectExplorer::ProductType::Other);
}

VOLCANO_EDITOR_END
