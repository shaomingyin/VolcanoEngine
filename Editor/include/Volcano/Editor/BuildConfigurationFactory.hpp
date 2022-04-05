//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_HPP
#define VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_HPP

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildConfigurationFactory: public ProjectExplorer::BuildConfigurationFactory {
public:
    BuildConfigurationFactory(void);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATIONFACTORY_HPP
