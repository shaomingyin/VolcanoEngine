//
//
#include <projectexplorer/buildinfo.h>

#include <Volcano/Editor/BuildConfiguration.hpp>
#include <Volcano/Editor/BuildConfigurationFactory.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target* target, Utils::Id id)
    : ProjectExplorer::BuildConfiguration(target, id) {
}

VOLCANO_EDITOR_END
