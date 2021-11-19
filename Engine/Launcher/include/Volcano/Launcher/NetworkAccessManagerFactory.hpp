//
//
#ifndef VOLCANO_LAUNCHER_NETWORKACCESSMANAGERFACTORY_HPP
#define VOLCANO_LAUNCHER_NETWORKACCESSMANAGERFACTORY_HPP

#include <QQmlNetworkAccessManagerFactory>

#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class NetworkAccessManagerFactory: public QQmlNetworkAccessManagerFactory {
public:
    NetworkAccessManagerFactory(void);
    ~NetworkAccessManagerFactory(void) override = default;

public:
    QNetworkAccessManager *create(QObject *parent) override;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_NETWORKACCESSMANAGERFACTORY_HPP
