//
//
#include <Volcano/Editor/ProjectNode.hpp>

VOLCANO_EDITOR_BEGIN

ProjectNode::ProjectNode(const Utils::FilePath& path)
    : ProjectExplorer::ProjectNode(path) {
}

void ProjectNode::build() {
}

VOLCANO_EDITOR_END
