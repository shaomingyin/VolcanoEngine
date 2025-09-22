//
//
#include <coreplugin/iwizardfactory.h>
#include <qtsupport/qtkitaspect.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectmanager.h>

#include <Volcano/Editor/Project.h>
#include <Volcano/Editor/ProjectWizardFactory.h>
#include <Volcano/Editor/Plugin.h>

VOLCANO_EDITOR_BEGIN

Utils::Result<> Plugin::initialize(const QStringList& arguments) {
    ProjectExplorer::ProjectManager::registerProjectType<Project>(Project::MimeType);

    auto kit_manager = ProjectExplorer::KitManager::instance();
    if (ProjectExplorer::KitManager::isLoaded()) {
        kitsRestored();
    } else {
        connect(kit_manager, &ProjectExplorer::KitManager::kitsLoaded, this, &Plugin::kitsRestored, Qt::SingleShotConnection);
    }

    Core::IWizardFactory::registerFactoryCreator([] {
        return new ProjectWizardFactory();
    });

    return Utils::ResultOk;
}

void Plugin::extensionsInitialized() {
}

Plugin::ShutdownFlag Plugin::aboutToShutdown() {
    return ShutdownFlag::SynchronousShutdown;
}

void Plugin::kitsRestored() {
    auto kits = ProjectExplorer::KitManager::kits();
    qDebug() << "kit count: " << kits.count();
    for (auto& kit: kits) {
        qDebug() << "kit: " << kit->displayName();
        auto keys = kit->allKeys();
        for (auto& key: keys) {
        }
    }
    // ProjectExplorer::KitManager::registerKit([](ProjectExplorer::Kit* kit) {
    //     kit->setAutoDetected(false);
    //     //kit->
    // }, "Teest");
}

VOLCANO_EDITOR_END
