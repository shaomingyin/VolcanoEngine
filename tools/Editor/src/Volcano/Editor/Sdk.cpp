//
//
#include <Volcano/Editor/Sdk.h>

VOLCANO_EDITOR_BEGIN

Sdk::Sdk(const QDir& dirpath, QObject* parent)
    : id_(Utils::Id::generate())
    , dirpath_(dirpath)
    , version_(1, 2, 3) {
    auto s = version_.toString();
    qDebug() << s;
}

Utils::Id Sdk::id() const {
    return id_;
}

const QDir& Sdk::path() const {
    return dirpath_;
}

const QVersionNumber& Sdk::version() const {
    return version_;
}

bool Sdk::isFitKit(const ProjectExplorer::Kit& kit) const {
    // TODO
    return true;
}

VOLCANO_EDITOR_END
