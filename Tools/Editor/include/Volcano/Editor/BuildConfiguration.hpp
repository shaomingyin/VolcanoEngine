//
//
#ifndef VOLCANO_EDITOR_BUILDCONFIGURATION_HPP
#define VOLCANO_EDITOR_BUILDCONFIGURATION_HPP

#include <projectexplorer/buildconfiguration.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildConfiguration: public ProjectExplorer::BuildConfiguration {
    Q_OBJECT

public:
    static constexpr const char Id[] = "Volcano.Editor.BuildConfiguration";

public:
    BuildConfiguration(ProjectExplorer::Target* target, Utils::Id id);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDCONFIGURATION_HPP
