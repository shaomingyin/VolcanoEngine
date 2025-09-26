//
//
#include <Volcano/World/Loadable.h>

VOLCANO_WORLD_BEGIN

Loadable::Loadable(QObject* parent)
    : Transformable(parent) {
}

QNetworkReply* Loadable::requestContent() {
    auto network_access_manager = networkAccessManager();
    if (network_access_manager != nullptr) {
        return network_access_manager->get(QNetworkRequest(source_));
    }
    return nullptr;
}

QDataStream& operator<<(QDataStream& s, const Loadable& v) {
    s << static_cast<const Transformable&>(v);
    // TODO
    return s;
}

QDataStream& operator>>(QDataStream& s, Loadable& v) {
    s >> static_cast<Transformable&>(v);
    // TODO
    return s;
}

VOLCANO_WORLD_END
