//
//
#include <QList>

#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kit.h>

#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/BuildConfiguration.hpp>
#include <Volcano/Editor/BuildConfigurationFactory.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfigurationFactory::BuildConfigurationFactory() {
    registerBuildConfiguration<BuildConfiguration>(BuildConfiguration::Id);

    setSupportedProjectType(Project::Id);
    setSupportedProjectMimeTypeName(Project::MimeType);

    setBuildGenerator([](const ProjectExplorer::Kit* kit, const Utils::FilePath& filepath, bool) {
        QList<ProjectExplorer::BuildInfo> build_info_list;

        qDebug() << kit->isSdkProvided();
        qDebug() << kit->allKeys();
        {
            auto keys = kit->allKeys();
            for (const auto& key: keys) {
                qDebug() << "  " << key << ": " << kit->value(key);
            }
        }
        qDebug() << kit->displayName();
        qDebug() << kit->id();
        qDebug() << kit->isValid();
        qDebug() << kit->supportedPlatforms();

        for (int i = 0; i < ProjectExplorer::BuildConfiguration::Release; ++i) {
            auto build_type = static_cast<ProjectExplorer::BuildConfiguration::BuildType>(i);
            auto path = filepath.absolutePath();
            ProjectExplorer::BuildInfo build_info;
            build_info.buildType = build_type;
            build_info.displayName = ProjectExplorer::BuildConfiguration::buildTypeName(build_type);
            build_info.buildDirectory = path.pathAppended("build_" + kit->unexpandedDisplayName() + "_" + build_info.displayName);
            //build_info.kitId = kit->id();
            build_info_list << build_info;
        }

        return build_info_list;
    });
}

VOLCANO_EDITOR_END
