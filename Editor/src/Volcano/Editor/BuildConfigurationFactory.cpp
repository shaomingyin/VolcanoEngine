//
//
#include <utils/filepath.h>
#include <projectexplorer/kit.h>
#include <projectexplorer/buildinfo.h>

#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/BuildConfiguration.hpp>
#include <Volcano/Editor/BuildConfigurationFactory.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfigurationFactory::BuildConfigurationFactory(void)
{
    registerBuildConfiguration<BuildConfiguration>(BuildConfiguration::ID);

    setSupportedProjectType(Project::ID);
    setSupportedProjectMimeTypeName(VOLCANO_EDITOR_PROJECT_MIMETYPE);

    setBuildGenerator([](const ProjectExplorer::Kit *k, const Utils::FilePath &projectPath, bool forSetup) {
        QList<ProjectExplorer::BuildInfo> result;

        auto debug = result.emplaceBack();
        debug.displayName = "Debug";
        debug.typeName = "Debug";
        debug.buildType = ProjectExplorer::BuildConfiguration::Debug;

        auto release = result.emplaceBack();
        release.displayName = "Release";
        release.typeName = "Release";
        release.buildType = ProjectExplorer::BuildConfiguration::Release;

        return result;
    });
}

VOLCANO_EDITOR_END
