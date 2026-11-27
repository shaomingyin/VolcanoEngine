#include <Volcano/World/SkyboxEnvironment.h>

VOLCANO_WORLD_BEGIN

SkyboxEnvironment::SkyboxEnvironment(QObject* parent)
    : Environment(parent) {
}

const QUrl& SkyboxEnvironment::source() const noexcept {
    return source_;
}

void SkyboxEnvironment::setSource(const QUrl& source) {
    if (source_ != source) {
        source_ = source;
        emit sourceChanged(source_);
    }
}

VOLCANO_WORLD_END
