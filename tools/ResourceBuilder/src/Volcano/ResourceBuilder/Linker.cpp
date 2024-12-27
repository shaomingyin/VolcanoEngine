//
//
#include <Volcano/ResourceBuilder/Linker.h>

VOLCANO_RESOURCEBUILDER_BEGIN

Linker::Linker(const QCommandLineParser& cmdline, QObject* parent)
    : Tool(cmdline, parent) {
}

int Linker::run() {
	return EXIT_SUCCESS;
}

VOLCANO_RESOURCEBUILDER_END
