//
//
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

#include <utils/async.h>
#include <utils/fileutils.h>
#include <coreplugin/progressmanager/progressmanager.h>

#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/BuildSystem.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath &filename)
    : ProjectExplorer::Project(VOLCANO_EDITOR_MIME_PROJECT_FILE, filename) {
    setDisplayName(filename.fileName());
    setBuildSystemCreator([](ProjectExplorer::Target* target) {
        return new BuildSystem(target);
    });
}

VOLCANO_EDITOR_END
