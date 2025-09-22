//
//
#include <Volcano/Editor/BuildConfiguration.h>

VOLCANO_EDITOR_BEGIN

BuildConfiguraton::BuildConfiguraton(ProjectExplorer::Target *target, Utils::Id id)
    : ProjectExplorer::BuildConfiguration(target, id) {
}

VOLCANO_EDITOR_END
