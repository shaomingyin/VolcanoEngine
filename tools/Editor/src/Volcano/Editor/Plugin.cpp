//
//
#include <coreplugin/iwizardfactory.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/projectmanager.h>
#include <cmakeprojectmanager/cmakeproject.h>

#include <Volcano/Editor/Plugin.h>

VOLCANO_EDITOR_BEGIN

Plugin::Plugin()
    : general_settings_page_(context_)
    , sdk_settings_page_(context_)
    , kit_aspect_factory_(context_)
    , scene_editor_factory_(context_)
    , property_view_(context_) {
}

Utils::Result<> Plugin::initialize(const QStringList& arguments) {
    Core::IOptionsPage::registerCategory(SETTINGS_CATEGORY, "VolcanoEngine", Utils::FilePath());

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

void Plugin::projectAdded(ProjectExplorer::Project* project) {
    auto cmake_project = qobject_cast<CMakeProjectManager::CMakeProject*>(project);
    if (cmake_project == nullptr) {
        return;
    }

    auto settings_path = cmake_project->projectDirectory().pathAppended(".volcano-settings");
    if (!settings_path.exists()) {
        return;
    }
}

void Plugin::projectRemoved(ProjectExplorer::Project* project) {
    auto cmake_project = qobject_cast<CMakeProjectManager::CMakeProject*>(project);
    if (cmake_project == nullptr) {
        return;
    }

    auto settings_path = cmake_project->projectDirectory().pathAppended(".volcano-settings");
    if (!settings_path.exists()) {
        return;
    }
}

VOLCANO_EDITOR_END
