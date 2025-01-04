//
//
#include <Volcano/ResourceBuilder/Packager.h>

VOLCANO_RESOURCEBUILDER_BEGIN

Packager::Packager(const QCommandLineParser& cmdline, QObject* parent)
    : Tool(cmdline, parent) {
    setObjectName("Packager");
}

void Packager::run() {
}

VOLCANO_RESOURCEBUILDER_END
