//
//
#include <Volcano/ResourceBuilder/Tool.h>

VOLCANO_RESOURCEBUILDER_BEGIN

Tool::Tool(const QCommandLineParser& cmdline, QObject* parent)
    : QObject(parent)
    , cmdline_(cmdline) {
}

VOLCANO_RESOURCEBUILDER_END
