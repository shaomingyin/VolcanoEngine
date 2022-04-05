//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATION_HPP
#define VOLCANO_EDITOR_BUILDCONFIGURATION_HPP

#include <utils/id.h>
#include <projectexplorer/target.h>
#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildConfiguration: public ProjectExplorer::BuildConfiguration {
    Q_OBJECT

public:
    static constexpr char ID[] = "VolcanoProjectManager.BuildConfiguration";

public:
    BuildConfiguration(ProjectExplorer::Target *target, Utils::Id id);
    ~BuildConfiguration(void) override;

public:
    ProjectExplorer::NamedWidget *createConfigWidget(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATION_HPP
