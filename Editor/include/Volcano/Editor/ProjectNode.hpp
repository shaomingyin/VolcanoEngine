//
//
#ifndef VOLCANO_EDITOR_PROJECTNODE_HPP
#define VOLCANO_EDITOR_PROJECTNODE_HPP

#include <utils/filepath.h>
#include <projectexplorer/projectnodes.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ProjectNode: public ProjectExplorer::ProjectNode {
public:
    ProjectNode(const Utils::FilePath &projectFilePath);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTNODE_HPP
