//
//
#include <projectexplorer/buildinfo.h>

#include <Volcano/Editor/BuildConfiguration.hpp>
#include <Volcano/Editor/BuildConfigurationFactory.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* target)
    : ProjectExplorer::BuildConfiguration(target, "VBC_TODO") {
}

VOLCANO_EDITOR_END
