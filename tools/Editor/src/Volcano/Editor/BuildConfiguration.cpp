//
//
#include <Volcano/Editor/BuildConfiguration.h>

VOLCANO_EDITOR_BEGIN

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target *target, Utils::Id id)
    : ProjectExplorer::BuildConfiguration(target, id)
    , build_system_(new BuildSystem(this)) {
    setInitializer([this, target](const ProjectExplorer::BuildInfo &info) {
    });
}

ProjectExplorer::BuildSystem* BuildConfiguration::buildSystem() const {
    return build_system_;
}

VOLCANO_EDITOR_END
