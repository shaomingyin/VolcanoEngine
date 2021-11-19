//
//
#ifndef VOLCANO_LAUNCHER_NETWORKACCESSMANAGER_HPP
#define VOLCANO_LAUNCHER_NETWORKACCESSMANAGER_HPP

#include <QIODevice>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

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

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_NETWORKACCESSMANAGER_HPP
