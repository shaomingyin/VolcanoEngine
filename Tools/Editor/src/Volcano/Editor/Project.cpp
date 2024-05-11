//
//
#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath& filename)
    : ProjectExplorer::Project("application/volcano-project", filename)
    , root_node_(filename.canonicalPath()) {
    setId("VolcanoProject");
    setCanBuildProducts();
    setNeedsBuildConfigurations(true);
    setNeedsDeployConfigurations(true);
    //setBuildSystemCreator();
}

ProjectExplorer::ProjectNode *Project::rootProjectNode() const {
    return nullptr; //&root_node_;
}

bool Project::needsConfiguration() const {
    return ProjectExplorer::Project::needsConfiguration();
}

bool Project::isEditModePreferred() const {
    return ProjectExplorer::Project::isEditModePreferred();
}

VOLCANO_EDITOR_END
