//
//
#ifndef VOLCANO_EDITOR_PROJECT_HPP
#define VOLCANO_EDITOR_PROJECT_HPP

#include <utils/filepath.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/project.h>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/ProjectNode.hpp>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    Project(const Utils::FilePath& filename);

public:
    ProjectExplorer::ProjectNode *rootProjectNode() const override;
    bool needsConfiguration() const override;
    bool isEditModePreferred() const override;

private:
    ProjectNode root_node_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
