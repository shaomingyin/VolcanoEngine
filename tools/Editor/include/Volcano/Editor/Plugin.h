//
//
#ifndef VOLCANO_EDITOR_PLUGIN_H
#define VOLCANO_EDITOR_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/KitSettings.h>
#include <Volcano/Editor/KitAspectFactory.h>
#include <Volcano/Editor/BuildConfigurationFactory.h>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "VolcanoEditor.json")

public:
    Plugin() = default;

public:
    Utils::Result<> initialize(const QStringList& arguments) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void kitsRestored();

private:
    KitAspectFactory kit_aspect_factory_;
    KitSettingsPage kit_settings_page_;
    BuildConfiguratonFactory build_configuration_factory_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_H
