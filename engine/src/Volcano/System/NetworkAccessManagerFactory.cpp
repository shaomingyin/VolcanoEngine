//
//
#include <memory>

#include <Volcano/System/NetworkAccessManager.hpp>
#include <Volcano/System/NetworkAccessManagerFactory.hpp>

VOLCANO_SYSTEM_BEGIN

NetworkAccessManagerFactory::NetworkAccessManagerFactory(const QString &rootDir, const QStringList &overlayList):
    m_rootDir(rootDir),
    m_overlayList(overlayList)
{
}

QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent)
{
    qDebug() << __FUNCTION__;

    auto p = std::make_unique<NetworkAccessManager>(parent);
    if (!p || !p->init(m_rootDir, m_overlayList))
        return nullptr;

    return p.release();
}

VOLCANO_SYSTEM_END
