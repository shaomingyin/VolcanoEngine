//
//
#include <Volcano/Net/Session.hpp>

VOLCANO_NET_BEGIN

Session::Session(QObject *parent):
    QObject(parent),
    m_enetPeer(nullptr)
{
}

Session::~Session(void)
{
    Q_ASSERT(m_enetPeer->data == this);
    m_enetPeer->data = nullptr;
}

bool Session::isMatch(const ENetPeer *peer) const
{

}

const QString &Session::host(void) const
{
    return m_host;
}

quint16 Session::port(void) const
{
    return m_port;
}

quint32 Session::pingInterval(void) const
{
    if (m_enetPeer != nullptr)
        return m_enetPeer->pingInterval;
    return 0;
}

void Session::setPingInterval(quint32 v)
{
    if (m_enetPeer == nullptr)
        return;

    if (m_enetPeer->pingInterval != v) {
        enet_peer_ping_interval(m_enetPeer, v);
        emit pingIntervalChanged(v);
    }
}

quint32 Session::rtt(void) const
{
    if (m_enetPeer != nullptr)
        return m_enetPeer->roundTripTime;
    return 0;
}

void Session::reset(void)
{
    if (m_enetPeer != nullptr)
        enet_peer_reset(m_enetPeer);
}

void Session::disconnect(void)
{
    if (m_enetPeer != nullptr)
        enet_peer_disconnect(m_enetPeer, 0);
}

bool Session::send(const QByteArray &data)
{
    if (m_enetPeer == nullptr)
        return false;

    auto packet = enet_packet_create(data.data(), data.size(), 0);
    if (packet == nullptr)
        return false;

    if (enet_peer_send(m_enetPeer, 0, packet) < 0)
        return false;

    return true;
}

bool Session::init(ENetPeer *peer)
{
    Q_ASSERT(peer != nullptr);
    Q_ASSERT(m_enetPeer == nullptr);

    m_enetPeer = peer;
    m_enetPeer->data = this;

    return true;
}

void Session::onReceived(const QByteArray &data)
{
    emit dataReceived(data);
}

Session *Session::fromENetPeer(ENetPeer *peer)
{
    if (peer != nullptr)
        return reinterpret_cast<Session *>(peer->data);
    return nullptr;
}

VOLCANO_NET_END
