//
//
#include <QDataStream>

#include <Volcano/System/Session.hpp>

VOLCANO_SYSTEM_BEGIN

Session::Session(ENetPeer *enetPeer, QObject *parent):
    QObject(parent),
    m_enetPeer(enetPeer)
{
    Q_ASSERT(m_enetPeer != nullptr);
    m_enetPeer->data = this;
}

Session::~Session(void)
{
}

ENetPeer *Session::toENetPeer(void)
{
    return m_enetPeer;
}

void Session::handleReceived(ENetPeer *enetPeer, const QByteArray &data)
{
    auto session = reinterpret_cast<Session *>(enetPeer->data);
    if (session != nullptr)
        session->onReceived(data);
}

void Session::onReceived(const QByteArray &data)
{
    Q_ASSERT(m_enetPeer != nullptr);

    QDataStream decoder(data);

    // TODO
}

VOLCANO_SYSTEM_END
