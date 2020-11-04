//
//
#include <Volcano/ResourceManager.hpp>

VOLCANO_BEGIN

ResourceManager::ResourceManager(QObject *parent):
    QNetworkAccessManager(parent)
{
}

ResourceManager::~ResourceManager(void)
{
}

QNetworkReply *ResourceManager::request(const QUrl &url, int timeout)
{
    Q_ASSERT(url.isValid());

    QNetworkRequest req(url);

    if (timeout > 0)
        req.setTransferTimeout(timeout);

    req.setAttribute(QNetworkRequest::CacheSaveControlAttribute, true);
    req.setAttribute(QNetworkRequest::AutoDeleteReplyOnFinishAttribute, false);
    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

    return get(req);
}

QStringList ResourceManager::supportedSchemesImplementation(void) const
{
    // TODO
    return QNetworkAccessManager::supportedSchemesImplementation();
}

QNetworkReply *ResourceManager::createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData)
{
    // TODO
    return QNetworkAccessManager::createRequest(op, originalReq, outgoingData);
}

VOLCANO_END
