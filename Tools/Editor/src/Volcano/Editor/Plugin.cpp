//
//
#include <QMenu>

#include <utils/mimeutils.h>
#include <utils/filepath.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projecttree.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/projectmanager.h>

#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/ProjectWizardFactory.hpp>
#include <Volcano/Editor/Plugin.hpp>

VOLCANO_EDITOR_BEGIN

bool Plugin::initialize(const QStringList& arguments, QString* error_string) {
    ProjectExplorer::ProjectManager::registerProjectType<Project>(VOLCANO_EDITOR_MIME_PROJECT_FILE);
    Core::IWizardFactory::registerFactoryCreator([] { return new ProjectWizardFactory(); });
    return true;
}

void Plugin::extensionsInitialized() {
}

ExtensionSystem::IPlugin::ShutdownFlag Plugin::aboutToShutdown() {
    return SynchronousShutdown;
}

VOLCANO_EDITOR_END
