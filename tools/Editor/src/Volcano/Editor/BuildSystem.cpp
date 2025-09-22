//
//
#include <Volcano/Editor/BuildSystem.h>

VOLCANO_EDITOR_BEGIN

BuildSystem::BuildSystem(ProjectExplorer::BuildConfiguration *build_configuration)
    : ProjectExplorer::BuildSystem(build_configuration) {
}

BuildSystem::~BuildSystem() {
}

void BuildSystem::triggerParsing() {
}

VOLCANO_EDITOR_END
