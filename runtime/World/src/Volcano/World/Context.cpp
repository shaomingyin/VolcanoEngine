//
//
#include <Volcano/World/Context.h>

VOLCANO_WORLD_BEGIN

Context::Context(QObject* parent)
    : QObject(parent)
    , fallback_network_access_manager_(nullptr) {
}

QNetworkAccessManager* Context::networkAccessManager(QObject* object) {
    auto qml_engine = qmlEngine(object);
    if (qml_engine != nullptr) {
        return qml_engine->networkAccessManager();
    }
    if (!fallback_network_access_manager_) {
        fallback_network_access_manager_.reset(createFallbackNetworkAccessManager());
    }
    return fallback_network_access_manager_.get();
}

QNetworkAccessManager* Context::createFallbackNetworkAccessManager() {
    return new QNetworkAccessManager();
}

VOLCANO_WORLD_END
