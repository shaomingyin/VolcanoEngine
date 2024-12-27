//
//
#include <Volcano/ResourceBuilder/ModelCompiler.h>

VOLCANO_RESOURCEBUILDER_BEGIN

ModelCompiler::ModelCompiler(const QCommandLineParser& cmdline, QObject* parent)
    : Tool(cmdline, parent) {
}

int ModelCompiler::run() {
	return EXIT_SUCCESS;
}

VOLCANO_RESOURCEBUILDER_END
