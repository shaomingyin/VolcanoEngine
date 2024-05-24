//
//
#include <projectexplorer/buildinfo.h>

#include <Volcano/Editor/BuildConfiguration.hpp>
#include <Volcano/Editor/BuildConfigurationFactory.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfigurationFactory::BuildConfigurationFactory() {
    //registerBuildConfiguration<>();
    setBuildGenerator(std::bind(&BuildConfigurationFactory::generator, this,
                                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

QList<ProjectExplorer::BuildInfo> BuildConfigurationFactory::generator(
    const ProjectExplorer::Kit* kit, const Utils::FilePath& path, bool) {
    return {};
}

VOLCANO_EDITOR_END
