//
//
#ifndef VOLCANO_EDITOR_PLUGIN_H
#define VOLCANO_EDITOR_PLUGIN_H

#include <extensionsystem/iplugin.h>
#include <projectexplorer/project.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>
#include <Volcano/Editor/GeneralSettings.h>
#include <Volcano/Editor/SdkSettings.h>
#include <Volcano/Editor/KitAspectFactory.h>
#include <Volcano/Editor/SceneEditorFactory.h>
#include <Volcano/Editor/PropertyView.h>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Editor.json")

public:
    Plugin();

public:
    Utils::Result<> initialize(const QStringList& arguments) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void kitsRestored();
    void projectAdded(ProjectExplorer::Project* project);
    void projectRemoved(ProjectExplorer::Project* project);

private:
    Context context_;
    GeneralSettingsPage general_settings_page_;
    SdkSettingsPage sdk_settings_page_;
    KitAspectFactory kit_aspect_factory_;
    SceneEditorFactory scene_editor_factory_;
    PropertyView property_view_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_H
