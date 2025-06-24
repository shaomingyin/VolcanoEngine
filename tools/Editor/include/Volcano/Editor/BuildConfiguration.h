//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATION_H
#define VOLCANO_EDITOR_BUILDCONFIGURATION_H

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/BuildSystem.h>

VOLCANO_EDITOR_BEGIN

class BuildConfiguration: public ProjectExplorer::BuildConfiguration {
    Q_OBJECT

public:
    explicit BuildConfiguration(ProjectExplorer::Target *target, Utils::Id id);

public:
    ProjectExplorer::BuildSystem* buildSystem() const override;

    BuildSystem* realBuildSystem() const {
        return build_system_;
    }

private:
    BuildSystem* build_system_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATION_H
