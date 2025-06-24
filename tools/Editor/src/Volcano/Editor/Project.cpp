//
//
#include <QList>

#include <qtsupport/qtkitaspect.h>
#include <qtsupport/qtversionmanager.h>

#include <coreplugin/icontext.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildinfo.h>
#include <projectexplorer/kitmanager.h>

#include <Volcano/Editor/BuildSystem.h>
#include <Volcano/Editor/Project.h>

VOLCANO_EDITOR_BEGIN

const Utils::Id Project::Id("VolcanoEditorProject");
const QString Project::MimeType(VOLCANO_EDITOR_PROJECT_MIMETYPE);

Project::Project(const Utils::FilePath &file_name)
    : ProjectExplorer::Project("", file_name) {
    setId(Id);
    setDisplayName(file_name.completeBaseName());
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::QMLJS_LANGUAGE_ID));
    setBuildSystemCreator<BuildSystem>();
}

void Project::configureAsExampleProject(ProjectExplorer::Kit* kit) {
}

VOLCANO_EDITOR_END
