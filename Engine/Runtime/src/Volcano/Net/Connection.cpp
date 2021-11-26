//
//
#include <QScopeGuard>

#include <Volcano/Net/Connection.hpp>

VOLCANO_NET_BEGIN

Connection::Connection(QObject *parent):
    Session(parent),
    m_enetHost(nullptr)
{
}

Connection::~Connection(void)
{
    if (m_enetHost != nullptr)
        enet_host_destroy(m_enetHost);
}

bool Connection::connect(void)
{
    if (m_enetHost != nullptr)
        return true;

    m_enetHost = enet_host_create(nullptr, 0, 0, 0, 0);
    if (m_enetHost == nullptr)
        return false;

    QScopeGuard hostGuard([this] {
        enet_host_destroy(m_enetHost);
        m_enetHost = nullptr;
    });

    ENetAddress addr;
    enet_address_set_host(&addr, qPrintable(host()));
    addr.port = port();

    auto peer = enet_host_connect(m_enetHost, &addr, 0, 0);
    if (peer == nullptr)
        return false;

    if (!init(peer))
        return false;

    hostGuard.dismiss();

    return true;
}

void Connection::close(void)
{

}

void Connection::onActivated(QSocketNotifier::Type type)
{
    Q_ASSERT(m_enetHost != nullptr);

    ENetEvent evt;

    int ret = enet_host_service(m_enetHost, &evt, 0);
    if (ret < 1)
        return;

    switch (evt.type) {
    case ENET_EVENT_TYPE_RECEIVE:
        onReceived(QByteArray::fromRawData(
                   (const char *)(evt.packet->data), evt.packet->dataLength));
        enet_packet_destroy(evt.packet);
        break;
    case ENET_EVENT_TYPE_CONNECT:
        setState(STATE_CONNECTED);
        break;
    case ENET_EVENT_TYPE_DISCONNECT:
        setState(STATE_DISCONNECTED);
        break;
    case ENET_EVENT_TYPE_NONE:
    default:
        break;
    }
}

void Connection::onReceived(const QByteArray &data)
{
    emit dataReceived(data);
}

VOLCANO_NET_END
