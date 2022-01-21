//
//
#ifndef VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
#define VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP

#include <QIODevice>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class NetworkAccessManager: public QNetworkAccessManager {
    Q_OBJECT

public:
    NetworkAccessManager(QObject *parent = nullptr);
    ~NetworkAccessManager(void) override;

public:
    QStringList supportedSchemes(void) const override;

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &originalReq, QIODevice *outgoingData) override;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_NETWORKACCESSMANAGER_HPP
