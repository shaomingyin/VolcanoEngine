//
//
#include <Volcano/Editor/Project.h>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath &fileName)
    : ProjectExplorer::Project("", fileName) {
}

VOLCANO_EDITOR_END
