//
//
#ifndef VOLCANO_EDITOR_PLUGIN_HPP
#define VOLCANO_EDITOR_PLUGIN_HPP

#include <extensionsystem/iplugin.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Volcano/Editor/Plugin.json")

public:
    Plugin(void);
    ~Plugin(void) override;

public:
    ShutdownFlag aboutToShutdown(void) override;
    void extensionsInitialized(void) override;
    bool initialize(const QStringList &arguments, QString *errorString) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_HPP
