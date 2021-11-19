//
//
#include <memory>

#include <Volcano/Launcher/NetworkAccessManager.hpp>
#include <Volcano/Launcher/NetworkAccessManagerFactory.hpp>

VOLCANO_LAUNCHER_BEGIN

NetworkAccessManagerFactory::NetworkAccessManagerFactory(void)
{
}

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    return new NetworkAccessManager(parent);
}

VOLCANO_LAUNCHER_END
