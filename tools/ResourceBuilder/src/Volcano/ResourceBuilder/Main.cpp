//
//
#include <memory>
#include <string>

#include <argh.h>

#include <Volcano/ResourceBuilder/Common.h>
#include <Volcano/ResourceBuilder/ModelCompiler.h>
#include <Volcano/ResourceBuilder/Linker.h>

VOLCANO_RESOURCEBUILDER_BEGIN

static void printVersion() {
	logInfo("VolcanoResourceBuilder - v" VOLCANO_VERSION_STR);
}

static void printHelp() {
	logInfo("Usage:");
	logInfo("  VolcanoResourceBuilder [options]");
	logInfo("Options:");
	logInfo("  -t, --tool [tool name]");
	logInfo("  -v, --version");
	logInfo("  -h, --help");
}

static int run(int argc, char* argv[]) {
	argh::parser cmdline(argv);

	printVersion();

	if (cmdline[{ "-v", "--version" }]) {
		return EXIT_SUCCESS;
	}

	if (cmdline[{ "-h", "--help" }]) {
		printHelp();
		return EXIT_SUCCESS;
	}

	std::string tool_name;
	std::unique_ptr<Tool> tool;
	cmdline({ "-t", "--tool" }) >> tool_name;
	if (tool_name == "model-compiler") {
		tool = std::make_unique<ModelCompiler>(cmdline);
	} else if (tool_name == "linker") {
		tool = std::make_unique<Linker>(cmdline);
	} else {
		logError("Invalid tool name: {}", tool_name);
		return EXIT_FAILURE;
	}

	if (!tool) {
		return EXIT_FAILURE;
	}

	return tool->run();
}

VOLCANO_RESOURCEBUILDER_END

int main(int argc, char* argv[]) {
	return Volcano::ResourceBuilder::run(argc, argv);
}
