//
//
#include <Volcano/Editor/Sdk.h>

VOLCANO_EDITOR_BEGIN

Sdk::Sdk(const QDir& dirpathm, QObject* parent)
    : QObject(parent) {
}

const QDir& Sdk::path() const {
    return dirpath_;
}

const QVersionNumber& Sdk::version() const {
    return version_;
}

VOLCANO_EDITOR_END
