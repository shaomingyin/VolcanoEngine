//
//
#include <qtsupport/qtversionmanager.h>
#include <coreplugin/iwizardfactory.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectmanager.h>

#include <Volcano/Editor/ProjectWizardFactory.h>
#include <Volcano/Editor/Project.h>
#include <Volcano/Editor/Plugin.h>

VOLCANO_EDITOR_BEGIN

bool Plugin::initialize(const QStringList &arguments, QString *errorString) {
    Core::IWizardFactory::registerFactoryCreator([] { return new ProjectWizardFactory; });
    ProjectExplorer::ProjectManager::registerProjectType<Project>(Project::MimeType);
    return true;
}

void Plugin::extensionsInitialized() {
    connect(ProjectExplorer::KitManager::instance(), &ProjectExplorer::KitManager::kitsLoaded, this, &Plugin::setupKits);
}

Plugin::ShutdownFlag Plugin::aboutToShutdown() {
    return ShutdownFlag::SynchronousShutdown;
}

void Plugin::setupKits() {
    auto init_kit = [](ProjectExplorer::Kit* k) {
        k->setUnexpandedDisplayName("Volcano");

        QtSupport::QtVersion *qt_version = QtSupport::QtVersionManager::version(0);
    };
    ProjectExplorer::KitManager::registerKit(init_kit);
}

VOLCANO_EDITOR_END
