//
//
#include <Volcano/World/Resource.h>

VOLCANO_WORLD_BEGIN

static void trueNetworkReplyDeleter(QNetworkReply* p) {
}

static void falseNetworkReplyDeleter(QNetworkReply* p) {
    delete p;
}

Resource::Resource(QObject* parent)
    : Transformable(parent) {
}

QNetworkAccessManager* Resource::networkAccessManager() {
    auto ctx = context();
    if (ctx != nullptr) {
        return ctx->networkAccessManager(this);
    }
    return nullptr;
}

QNetworkReply* Resource::requestContent() {
    auto net = networkAccessManager();
    if (net != nullptr) {
        return net->get(QNetworkRequest(source_));
    }
    return nullptr;
}

VOLCANO_WORLD_END
