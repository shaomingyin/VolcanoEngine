//
//
#ifndef VOLCANO_EDITOR_PLUGIN_H
#define VOLCANO_EDITOR_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "VolcanoEditor.json")

public:
    Plugin() = default;

public:
    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void setupKits();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_H
