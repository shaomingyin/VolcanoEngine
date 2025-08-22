//
//
#include <Volcano/Editor/BuildSystem.h>

VOLCANO_EDITOR_BEGIN

BuildSystem::BuildSystem(ProjectExplorer::Target *target)
    : ProjectExplorer::BuildSystem(target) {
}

BuildSystem::BuildSystem(ProjectExplorer::BuildConfiguration *build_configuration)
    : ProjectExplorer::BuildSystem(build_configuration) {
}

BuildSystem::~BuildSystem() {
}

QString BuildSystem::name() const {
    return "VolcanoBuildSystem";
}

void BuildSystem::triggerParsing() {
}

VOLCANO_EDITOR_END
