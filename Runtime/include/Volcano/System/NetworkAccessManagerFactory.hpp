//
//
#ifndef VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP
#define VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP

#include <QQmlNetworkAccessManagerFactory>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class NetworkAccessManagerFactory: public QQmlNetworkAccessManagerFactory {
public:
    NetworkAccessManagerFactory(void);
    ~NetworkAccessManagerFactory(void) override = default;

public:
    QNetworkAccessManager *create(QObject *parent) override;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_NETWORKACCESSMANAGERFACTORY_HPP
