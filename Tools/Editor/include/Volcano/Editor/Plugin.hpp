//
//
#ifndef VOLCANO_EDITOR_PLUGIN_HPP
#define VOLCANO_EDITOR_PLUGIN_HPP

#include <extensionsystem/iplugin.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class Plugin: public ExtensionSystem::IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "shaomy.VolcanoEngine.EditorPlugin" FILE "Volcano/Editor/Plugin.json")

public:
    bool initialize(const QStringList &arguments, QString *error_string) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PLUGIN_HPP
