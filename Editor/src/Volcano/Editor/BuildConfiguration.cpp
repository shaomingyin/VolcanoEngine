//
//
#include <Volcano/Editor/BuildConfigurationWidget.hpp>
#include <Volcano/Editor/BuildConfiguration.hpp>

VOLCANO_EDITOR_BEGIN

BuildConfiguration::BuildConfiguration(ProjectExplorer::Target *target, Utils::Id id):
    ProjectExplorer::BuildConfiguration(target, id)
{
}

BuildConfiguration::~BuildConfiguration(void)
{
}

ProjectExplorer::NamedWidget *BuildConfiguration::createConfigWidget(void)
{
    return new BuildConfigurationWidget(this);
}

VOLCANO_EDITOR_END
