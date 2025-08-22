//
//
#include <Volcano/Editor/BuildSystem.h>
#include <Volcano/Editor/Project.h>

VOLCANO_EDITOR_BEGIN

const QString Project::MimeType(VOLCANO_EDITOR_MIMETYPE_PROJECT_FILE);

Project::Project(const Utils::FilePath& filename)
    : ProjectExplorer::Project(MimeType, filename) {
    setBuildSystemCreator<BuildSystem>();
}

VOLCANO_EDITOR_END
