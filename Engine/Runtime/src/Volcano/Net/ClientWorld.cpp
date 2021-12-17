//
//
#include <QDataStream>
#include <QScopeGuard>

#include <Volcano/Net/ClientWorld.hpp>

VOLCANO_NET_BEGIN

ClientWorld::ClientWorld(QObject *parent):
    Game::WorldBase(parent),
    m_state(STATE_DISABLED),
    m_url("volcano://localhost"),
    m_rxNotifier(QSocketNotifier::Read),
    m_enetHost(nullptr),
    m_enetPeer(nullptr)
{
    connect(&m_rxNotifier, &QSocketNotifier::activated, this, &ClientWorld::onSocketActivated);
}

ClientWorld::~ClientWorld(void)
{
    if (m_enetHost != nullptr)
        enet_host_destroy(m_enetHost);
}

ClientWorld::State ClientWorld::state(void) const
{
    return m_state;
}

bool ClientWorld::isEnabled() const
{
    return (m_state == STATE_ENABLED);
}

void ClientWorld::setEnabled(bool v)
{
    if (isEnabled()) {
        if (!v)
            disable();
    } else {
        if (v)
            enable();
    }
}

const QUrl &ClientWorld::url(void) const
{
    return m_url;
}

void ClientWorld::setUrl(const QUrl &v)
{
    if (m_url == v || isEnabled()) {
        m_url = v;
        emit urlChanged(v);
    }
}

bool ClientWorld::enable(void)
{
    if (m_state != STATE_DISABLED)
        return false;

    if (m_url.scheme().compare("volcano", Qt::CaseInsensitive))
        return false;

    Q_ASSERT(m_enetHost == nullptr);
    m_enetHost = enet_host_create(nullptr, 0, 0, 0, 0);
    if (m_enetHost == nullptr)
        return false;

    QScopeGuard hostGuard([this] {
        enet_host_destroy(m_enetHost);
        m_enetHost = nullptr;
    });

    ENetAddress addr;
    enet_address_set_host(&addr, qPrintable(m_url.host()));
    addr.port = m_url.port(DEFAULT_PORT);

    Q_ASSERT(m_enetPeer == nullptr);
    m_enetPeer = enet_host_connect(m_enetHost, &addr, 0, 0);
    if (m_enetPeer == nullptr)
        return false;

    setState(STATE_ENABLING);
    hostGuard.dismiss();

    return true;
}

void ClientWorld::disable(void)
{
    if (m_state != STATE_ENABLED)
        return;

    Q_ASSERT(m_enetHost != nullptr);
    Q_ASSERT(m_enetPeer != nullptr);

    enet_peer_disconnect(m_enetPeer, 0);
    setState(STATE_DISABLING);
}

void ClientWorld::setState(State v)
{
    if (m_state == v)
        return;

    m_state = v;
    emit stateChanged(v);

    switch (v) {
    case STATE_ENABLED:
        emit enabledChanged(true);
        break;
    case STATE_DISABLED:
        emit enabledChanged(false);
        break;
    default:
        break;
    }
}

void ClientWorld::onPeerConnected(ENetPeer *enetPeer)
{
    if (m_state != STATE_ENABLING)
        return;

    Q_ASSERT(m_enetHost != nullptr);
    Q_ASSERT(m_enetPeer == nullptr);

    m_enetPeer = enetPeer;
    setState(STATE_ENABLED);
}

void ClientWorld::onPeerDisconnected(ENetPeer *enetPeer)
{
    if (m_state != STATE_ENABLED)
        return;

    Q_ASSERT(m_enetPeer != nullptr);
    Q_ASSERT(m_enetPeer == enetPeer);

    m_enetPeer = nullptr;
    setState(STATE_DISABLED);
}

void ClientWorld::onPeerReceived(ENetPeer *enetPeer, const void *p, quint64 len)
{
    if (m_state != STATE_ENABLED)
        return;

    Q_ASSERT(m_enetPeer != nullptr);
    Q_ASSERT(m_enetPeer == enetPeer);

    auto data = QByteArray::fromRawData((const char *)p, len);
    QDataStream decoder(data);

    // TODO
}

void ClientWorld::onSocketActivated(QSocketDescriptor sd, QSocketNotifier::Type type)
{
    Q_ASSERT(m_enetHost != nullptr);
    Q_ASSERT(m_enetHost->socket == qintptr(sd));

    ENetEvent evt;
    int ret = enet_host_service(m_enetHost, &evt, 0);
    if (ret > 0) {
        switch (evt.type) {
        case ENET_EVENT_TYPE_RECEIVE:
            onPeerReceived(evt.peer, evt.packet->data, evt.packet->dataLength);
            enet_packet_destroy(evt.packet);
            break;
        case ENET_EVENT_TYPE_CONNECT:
            onPeerConnected(evt.peer);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            onPeerDisconnected(evt.peer);
            break;
        default:
            break;
        }
    }
}

VOLCANO_NET_END
