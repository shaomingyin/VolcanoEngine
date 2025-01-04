//
//
#include <Volcano/World/Loadable.h>

VOLCANO_WORLD_BEGIN

Loadable::Loadable(QObject* parent)
    : Transformable(parent) {
}

QNetworkAccessManager* Loadable::networkAccessManager() {
    auto ctx = context();
    if (ctx != nullptr) {
        return ctx->networkAccessManager(this);
    }
    return nullptr;
}

QNetworkReply* Loadable::requestContent() {
    auto net = networkAccessManager();
    if (net != nullptr) {
        return net->get(QNetworkRequest(source_));
    }
    return nullptr;
}

VOLCANO_WORLD_END
