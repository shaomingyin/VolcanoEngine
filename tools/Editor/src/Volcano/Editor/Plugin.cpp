//
//
#include <Volcano/Editor/Plugin.h>

VOLCANO_EDITOR_BEGIN

bool Plugin::initialize(const QStringList &arguments, QString *errorString) {
    return true;
}

void Plugin::extensionsInitialized() {
}

Plugin::ShutdownFlag Plugin::aboutToShutdown() {
    return ShutdownFlag::SynchronousShutdown;
}

VOLCANO_EDITOR_END
