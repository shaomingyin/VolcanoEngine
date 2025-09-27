//
//
#include <coreplugin/iwizardfactory.h>
#include <qtsupport/qtkitaspect.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectmanager.h>

#include <Volcano/Editor/Plugin.h>

VOLCANO_EDITOR_BEGIN

Plugin::Plugin()
    : general_settings_page_(context_)
    , kit_aspect_factory_(context_)
    , scene_editor_factory_(context_)
    , property_view_(context_) {
}

Utils::Result<> Plugin::initialize(const QStringList& arguments) {
    auto kit_manager = ProjectExplorer::KitManager::instance();
    if (ProjectExplorer::KitManager::isLoaded()) {
        kitsRestored();
    } else {
        connect(kit_manager, &ProjectExplorer::KitManager::kitsLoaded, this, &Plugin::kitsRestored, Qt::SingleShotConnection);
    }

    Core::IOptionsPage::registerCategory(SETTINGS_CATEGORY_ID, "VolcanoEngine", Utils::FilePath());

    auto pm = ProjectExplorer::ProjectManager::instance();
    connect(pm, &ProjectExplorer::ProjectManager::projectAdded, this, &Plugin::projectAdded);
    connect(pm, &ProjectExplorer::ProjectManager::projectRemoved, this, &Plugin::projectRemoved);

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

void Plugin::projectAdded(ProjectExplorer::Project* project) {

}

void Plugin::projectRemoved(ProjectExplorer::Project* project) {

}

VOLCANO_EDITOR_END
