//
//
#include <Volcano/Editor/BuildSystem.hpp>

VOLCANO_EDITOR_BEGIN


BuildSystem::BuildSystem(ProjectExplorer::Target* target)
    : ProjectExplorer::BuildSystem(target) {
}

void BuildSystem::triggerParsing() {
}

VOLCANO_EDITOR_END
