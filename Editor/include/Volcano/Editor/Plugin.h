//
//
#ifndef VOLCANO_EDITOR_PLUGIN_H
#define VOLCANO_EDITOR_PLUGIN_H

#include <extensionsystem/iplugin.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Volcano/Editor/Plugin.json")

public:
    Plugin(void);
    ~Plugin(void) override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized(void) override;
    ShutdownFlag aboutToShutdown(void) override;

private:
    void triggerAction(void);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_H
