//
//
#ifndef VOLCANO_EDTIOR_BUILDCONFIGURATION_H
#define VOLCANO_EDTIOR_BUILDCONFIGURATION_H

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class BuildConfiguraton: public ProjectExplorer::BuildConfiguration {
    Q_OBJECT

public:
    explicit BuildConfiguraton(ProjectExplorer::Target *target, Utils::Id id);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDTIOR_BUILDCONFIGURATION_H
