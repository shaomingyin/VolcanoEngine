//
//
#include <Volcano/Editor/Plugin.hpp>

VOLCANO_EDITOR_BEGIN

bool Plugin::initialize(const QStringList& arguments, QString* error_string) {
    return true;
}

void Plugin::extensionsInitialized() {
}

ExtensionSystem::IPlugin::ShutdownFlag Plugin::aboutToShutdown() {
    return SynchronousShutdown;
}

VOLCANO_EDITOR_END
