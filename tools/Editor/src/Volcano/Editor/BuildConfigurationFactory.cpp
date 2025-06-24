//
//
#include <QList>

#include <projectexplorer/kit.h>
#include <projectexplorer/buildinfo.h>

#include <Volcano/Editor/Project.h>
#include <Volcano/Editor/BuildConfigurationFactory.h>

VOLCANO_EDITOR_BEGIN

BuildConfigurationFactory::BuildConfigurationFactory() {
    setDefaultDisplayName("Volcano");
    setSupportedProjectType(Project::Id);
    setSupportedProjectMimeTypeName(Project::MimeType);

    setBuildGenerator([](const ProjectExplorer::Kit *kit, const Utils::FilePath &project_path, bool for_setup) {
        QList<ProjectExplorer::BuildInfo> build_info_list;

        ProjectExplorer::BuildInfo debug;
        debug.typeName = "Debug";
        debug.displayName = "Debug";
        debug.buildDirectory = for_setup ? project_path.absolutePath() : project_path; // TODO + /Debug
        build_info_list << debug;

        ProjectExplorer::BuildInfo release;
        release.typeName = "Release";
        release.displayName = "Release";
        release.buildDirectory = for_setup ? project_path.absolutePath() : project_path; // TODO + /Release
        build_info_list << release;

        return build_info_list;
    });
}

VOLCANO_EDITOR_END
