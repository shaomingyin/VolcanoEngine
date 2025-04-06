//
//
#include <Volcano/ResourceBuilder/Linker.h>

VOLCANO_RESOURCEBUILDER_BEGIN

Linker::Linker(const argh::parser& cmdline)
	: Tool(cmdline) {
}

int Linker::run() {
	return EXIT_SUCCESS;
}

VOLCANO_RESOURCEBUILDER_END
