//
//
#include <memory>

#include <Volcano/Launcher/NetworkAccessManager.hpp>

VOLCANO_LAUNCHER_BEGIN

NetworkAccessManager::NetworkAccessManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

NetworkAccessManager::~NetworkAccessManager(void)
{
}

QStringList NetworkAccessManager::supportedSchemes(void) const
{
    return (QNetworkAccessManager::supportedSchemes() << "vfs");
}

QNetworkReply *NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData)
{
    auto url = originalReq.url();
    if (url.scheme() != "vfs")
        return QNetworkAccessManager::createRequest(op, originalReq, outgoingData);

    // TODO

    return nullptr;
}

VOLCANO_LAUNCHER_END
