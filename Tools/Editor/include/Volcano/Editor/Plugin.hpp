//
//
#ifndef VOLCANO_EDITOR_PLUGIN_HPP
#define VOLCANO_EDITOR_PLUGIN_HPP

#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/SceneTreeEditorFactory.hpp>
#include <Volcano/Editor/SceneViewEditorFactory.hpp>
#include <Volcano/Editor/SettingsPage.hpp>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Volcano/Editor/Plugin.json")

public:
    bool initialize(const QStringList &arguments, QString *error_string) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    SceneTreeEditorFactory scene_tree_editor_factory_;
    SceneViewEditorFactory scene_view_editor_factory_;
    SettingsPage settings_page_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_HPP
