//
//
#include <Volcano/ResourceBuilder/ModelCompiler.h>

VOLCANO_RESOURCEBUILDER_BEGIN

ModelCompiler::ModelCompiler(const argh::parser& cmdline)
	: Tool(cmdline) {
}

int ModelCompiler::run() {
	return EXIT_SUCCESS;
}

VOLCANO_RESOURCEBUILDER_END
