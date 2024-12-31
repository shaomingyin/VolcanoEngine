//
//
#include <QtConcurrent>

#include <Volcano/ResourceBuilder/Tool.h>

VOLCANO_RESOURCEBUILDER_BEGIN

Tool::Tool(const QCommandLineParser& cmdline, QObject* parent)
    : QObject(parent)
    , cmdline_(cmdline) {
}

void Tool::init() {
    future_ = QtConcurrent::run([this] {
        run();
    }).then(this, [this] {
        finished();
    }).onCanceled(this, [this] {
        canceled();
    }).onFailed(this, [this] {
        failed("");
    });
}

VOLCANO_RESOURCEBUILDER_END
