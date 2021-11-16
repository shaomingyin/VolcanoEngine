//
//
#include <memory>

#include <Volcano/NetworkAccessManager.hpp>
#include <Volcano/NetworkAccessManagerFactory.hpp>

VOLCANO_BEGIN

NetworkAccessManagerFactory::NetworkAccessManagerFactory(const QString &rootDir, const QStringList &overlayList):
    m_rootDir(rootDir),
    m_overlayList(overlayList)
{
}

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    qDebug() << __FUNCTION__;

    auto p = std::make_unique<NetworkAccessManager>(parent);
    if (!p || !p->init(m_rootDir))
        return nullptr;

    return p.release();
}

VOLCANO_END
