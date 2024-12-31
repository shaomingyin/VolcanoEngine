//
//
#include <Volcano/ResourceBuilder/ModelCompiler.h>

VOLCANO_RESOURCEBUILDER_BEGIN

ModelCompiler::ModelCompiler(const QCommandLineParser& cmdline, QObject* parent)
    : Tool(cmdline, parent) {
}

QString ModelCompiler::name() const {
    return "ModelCompiler";
}

void ModelCompiler::run() {
}

VOLCANO_RESOURCEBUILDER_END
