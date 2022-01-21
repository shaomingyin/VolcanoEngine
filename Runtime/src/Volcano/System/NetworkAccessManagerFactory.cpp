//
//
#include <memory>

#include <Volcano/System/NetworkAccessManager.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

NetworkAccessManagerFactory::NetworkAccessManagerFactory(void)
{
}

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    return new NetworkAccessManager(parent);
}

VOLCANO_SYSTEM_END
