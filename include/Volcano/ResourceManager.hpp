//
//
#ifndef VOLCANO_RESOURCEMANAGER_HPP
#define VOLCANO_RESOURCEMANAGER_HPP

#include <QUrl>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API ResourceManager: public QNetworkAccessManager
{
    Q_OBJECT

public:
    ResourceManager(QObject *parent = nullptr);
    ~ResourceManager(void) override;

public:
    QNetworkReply *request(const QUrl &url, int timeout = 30000);
    QNetworkReply *request(const QString &filename, int timeout = 30000);

protected slots:
    QStringList supportedSchemesImplementation(void) const;

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData) override;
};

VOLCANO_INLINE QNetworkReply *ResourceManager::request(const QString &filename, int timeout)
{
    return request(QUrl::fromLocalFile(filename), timeout);
}

VOLCANO_END

#endif // VOLCANO_RESOURCEMANAGER_HPP
