//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_H
#define VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_H

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class BuildConfigurationFactory: public ProjectExplorer::BuildConfigurationFactory {
public:
    BuildConfigurationFactory();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_H
