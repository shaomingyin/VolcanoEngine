//
//
#include <Volcano/ResourceBuilder/ModelCompiler.h>

VOLCANO_RESOURCEBUILDER_BEGIN

ModelCompiler::ModelCompiler(const QCommandLineParser& cmdline, QObject* parent)
    : Tool(cmdline, parent) {
    setObjectName("ModelCompiler");
}

void ModelCompiler::run() {
}

VOLCANO_RESOURCEBUILDER_END
