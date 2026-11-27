#include <Volcano/World/Model.h>

VOLCANO_WORLD_BEGIN

Model::Model(QObject* parent)
    : QObject(parent) {
}

const QUrl& Model::source() const noexcept {
    return source_;
}

void Model::setSource(const QUrl& source) {
    if (source_ != source) {
        source_ = source;
        emit sourceChanged(source_);
    }
}

VOLCANO_WORLD_END
