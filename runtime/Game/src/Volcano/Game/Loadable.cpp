//
//
#include <Volcano/Game/Loadable.h>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END
