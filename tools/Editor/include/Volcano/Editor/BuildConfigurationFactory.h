//
//
#ifndef VOLCANO_EDTIOR_BUILDCONFIGURATIONFACTORY_H
#define VOLCANO_EDTIOR_BUILDCONFIGURATIONFACTORY_H

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class BuildConfiguratonFactory: public ProjectExplorer::BuildConfigurationFactory {
public:
    BuildConfiguratonFactory();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDTIOR_BUILDCONFIGURATIONFACTORY_H
