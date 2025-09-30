//
//
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

Context::Context(QObject* parent)
    : QObject(parent) {
}

SdkManager& Context::sdkManager() {
    return sdk_manager_;
}

VOLCANO_EDITOR_END
