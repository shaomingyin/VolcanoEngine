//
//
#include <memory>
#include <string>

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/ModelCompiler.h>
#include <Volcano/ResourceBuilder/Linker.h>

VOLCANO_RESOURCEBUILDER_BEGIN

static void printVersion() {
    // logInfo("VolcanoResourceBuilder - v" VOLCANO_VERSION_STR);
}

static void printHelp() {
    // logInfo("Usage:");
    // logInfo("  VolcanoResourceBuilder [options]");
    // logInfo("Options:");
    // logInfo("  -t, --tool [tool name]");
    // logInfo("  -v, --version");
    // logInfo("  -h, --help");
}

static int run(int argc, char* argv[]) {
    return 0;
}

VOLCANO_RESOURCEBUILDER_END

int main(int argc, char* argv[]) {
	return Volcano::ResourceBuilder::run(argc, argv);
}
